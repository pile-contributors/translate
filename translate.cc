/**
 * @file translate.cc
 * @brief Definitions for translate class.
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2014 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#include "translate.h"
#include "translate-private.h"

#include <QCoreApplication>
#include <QDir>
#include <QObject>
#include <QSettings>
#include <QLocale>
#include <QDebug>
#include <QProcess>

#ifdef SQLITE3_FOUND
#   include <sqlite/sqlite3.h>
#endif // SQLITE3_FOUND

/**
 * @class Translate
 *
 * ...
 */

Translate * Translate::uniq_ = NULL;
static TransLang invalid;
#define METADATA_FILE "metadata.ini"
#define METADATA_GROUP "General"
#define METADATA_LABEL "label"
#define METADATA_ICON "icon"
#define METADATA_LANG "translate"
#define METADATA_LOCALE "locale"


/* ------------------------------------------------------------------------- */
/**
 * ...
 */
Translate::Translate() :
    d_ (),
    current_ (-1)
{
    TRANSLATE_TRACE_ENTRY;

    TRANSLATE_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
/**
 * ...
 */
Translate::~Translate()
{
    TRANSLATE_TRACE_ENTRY;

    TRANSLATE_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
/**
 * ...
 */
bool Translate::init (const char * env_var_path, QString * error)
{
    bool b_ret = false;
    TRANSLATE_TRACE_ENTRY;
    for (;;) {
        // create instance or reinitialize it
        if (uniq_ == NULL) {
            uniq_ = new Translate();
        } else {
            uniq_->d_.clear();
        }

        // enter the path
        QDir path;
        QString env_lang;
        if (env_var_path != NULL) {
            env_lang = QString::fromUtf8 (getenv (env_var_path)).trimmed ();
        }
        if (!env_lang.isEmpty() && QFileInfo(env_lang).isDir()) {
            path = QDir (env_lang);
        } else {
            if (QCoreApplication::instance() == NULL) {
                path = QDir::currentPath ();
            } else {
                path = QCoreApplication::applicationDirPath ();
            }

            if (!path.exists(QLatin1String("."))) {
                TRANSLATE_DEBUGM("Application folder does not exist: %s\n",
                                 TMP_A(path.absolutePath()));
                break;
            }
            TRANSLATE_DEBUGM("Aplication directory is %s\n",
                             TMP_A(path.absolutePath()));

            QString rel_path =
    #       ifdef TARGET_SYSTEM_APPLE
            "Contents/Resources/translations"
    #       else // TARGET_SYSTEM_APPLE
            QString("../share/locale/%1").arg(QCoreApplication::applicationName().toLower())
    #       endif // TARGET_SYSTEM_APPLE
            ;
            bool b_cd = path.cd (rel_path);
            if (!b_cd || !path.exists(".")) {
                error->append (QObject::tr (
                                   "Could not find translations directory %1\n").arg (
                                       TMP_A(rel_path)));
                break;
            }
        }
        TRANSLATE_DEBUGM("Translations directory is %s\n",
                         TMP_A(path.absolutePath()));

        // get all sub-directories and iterate them
        QStringList dirs = path.entryList (
                    QStringList("*"), QDir::Dirs,
                    QDir::IgnoreCase | QDir::Name);
        TRANSLATE_DEBUGM("Iterating over %d translations\n", dirs.count());

        foreach(const QString & s_dir, dirs) {
            if (!s_dir.compare (QLatin1String (".")))
                continue;
            if (!s_dir.compare (QLatin1String ("..")))
                continue;

            TRANSLATE_DEBUGM("  - %s\n", TMP_A(s_dir));

            QDir subdir (path.absoluteFilePath (s_dir));
            if (!subdir.exists (METADATA_FILE)) {
                TRANSLATE_DEBUGM("Folder without metadata: %s\n", TMP_A(s_dir));
                continue;
            }

            // read metadata
            QString s_meta_file = subdir.absoluteFilePath (METADATA_FILE);
            QSettings metadata (
                        s_meta_file,
                        QSettings::IniFormat);
            // metadata.beginGroup (METADATA_GROUP);

            // get some values we expect
            QString s_key_lbl (METADATA_LABEL);
            QString s_key_icon (METADATA_ICON);
            QString s_key_lang (METADATA_LANG);
            QString s_key_locale (METADATA_LOCALE);
            QString s_lbl (metadata.value (s_key_lbl).toString ().trimmed());
            QString s_icon (metadata.value (s_key_icon).toString ().trimmed());
            QString s_lang (metadata.value (s_key_lang).toString ().trimmed());
            QString s_locale (metadata.value (s_key_locale).toString ().trimmed());

            // a name must be provided or we declare it invalid
            if (s_lbl.isEmpty()) {
                TRANSLATE_DEBUGM("no name provided in metadata for %s\n",
                                 TMP_A(s_dir));
                continue;
            }
            // a language must be provided or we declare it invalid
            if (s_lang.isEmpty()) {
                TRANSLATE_DEBUGM("no language file provided in metadata for %s\n",
                                 TMP_A(s_dir));
                continue;
            }
            TRANSLATE_DEBUGM("    - name: %s\n", TMP_A(s_lbl));

            QString s_lang_full = subdir.absoluteFilePath (s_lang);

            // If the file does not exist in asigned directory we make an
            // attempt to locate it in a specially provided one.
            QFileInfo fi_lang (s_lang);
            if (!fi_lang.isFile()) {
                QString s_lang_ext (s_lang + ".qm");
                fi_lang.setFile (s_lang_ext );
                if (!fi_lang.isFile()) {
                    QString s_alternative_path =
                            QString::fromUtf8 (getenv ("TRANSLATE_QM_DIR")).trimmed ();
                    QDir alternative_path (s_alternative_path);
                    if (
                            alternative_path.exists (s_lang) ||
                            alternative_path.exists (s_lang_ext)) {

                        s_lang_full = alternative_path.absoluteFilePath (s_lang);
                    }
                }
            }
            s_lang = s_lang_full;
            TRANSLATE_DEBUGM("    - language path: %s\n", TMP_A(s_lang));

            if (!s_icon.isEmpty()) {
                s_icon = subdir.absoluteFilePath (s_icon);
            }
            TRANSLATE_DEBUGM("    - icon path: %s\n", TMP_A(s_icon));

            // we got ourselves an instance
            TransLang newinst (subdir.absolutePath(),
                               s_lang, s_lbl, s_icon, s_locale);

            // read other metadata
            foreach(const QString & s_key, metadata.allKeys()) {
                if (!s_key.compare(s_key_lbl, Qt::CaseInsensitive)) {
                    continue;
                } else if (!s_key.compare(s_key_icon, Qt::CaseInsensitive)) {
                    continue;
                } else if (!s_key.compare(s_key_lang, Qt::CaseInsensitive)) {
                    continue;
                } else if (!s_key.compare(s_key_locale, Qt::CaseInsensitive)) {
                    continue;
                }
                newinst.addMetadata (s_key, metadata.value (s_key).toString ());
            }

            // metadata.endGroup();

            uniq_->d_.append (newinst);
        }

        b_ret = true;
        break;
    }
    TRANSLATE_TRACE_EXIT;
    return b_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
/**
 * ...
 */
void Translate::end()
{
    TRANSLATE_TRACE_ENTRY;
    if (uniq_ != NULL) {
        delete uniq_;
        uniq_ = NULL;
    }
    TRANSLATE_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
const TransLang & Translate::item (int i)
{
    if (uniq_ == NULL)
        return invalid;
    if ((i < 0) || (i >= uniq_->d_.count())) {
        return invalid;
    }
    return uniq_->d_.at (i);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
const TransLang & Translate::item (const QString & s_name)
{
    if (uniq_ == NULL)
        return invalid;
    foreach(const TransLang & itm, uniq_->d_) {
        if (!itm.langName ().compare (s_name, Qt::CaseInsensitive)) {
            return itm;
        }
    }
    return invalid;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
int Translate::itemIndex(const QString &s_name)
{
    if (uniq_ == NULL)
        return -1;
    int i = 0;
    foreach(const TransLang & itm, uniq_->d_) {
        if (!itm.langName ().compare (s_name, Qt::CaseInsensitive)) {
            return i;
        }
        ++i;
    }
    return -1;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
int Translate::itemIndexFromLocale (const QString & s_locale)
{
    if (uniq_ == NULL)
        return -1;
    int i = 0;

    // attempt to find using both language and country
    foreach(const TransLang & itm, uniq_->d_) {
        if (!itm.localeName ().compare (s_locale, Qt::CaseInsensitive)) {
            return i;
        }
        ++i;
    }

    // attempt to find using only the language if there are two parts
    QStringList lang_comp = s_locale.split("_");
    if (lang_comp.count() <= 1)
        return -1;
    const QString s_locale_alone = lang_comp.at (0);
    i = 0;
    foreach(const TransLang & itm, uniq_->d_) {
        if (!itm.localeName ().compare (s_locale_alone, Qt::CaseInsensitive)) {
            return i;
        }
        ++i;
    }
    return -1;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool Translate::validInstance (const TransLang & item)
{
    return item != invalid;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QTranslator * Translate::translator (int i)
{
    if (uniq_ == NULL)
        return NULL;
    if ((i < 0) || (i >= uniq_->d_.count())) {
        return NULL;
    }
    return uniq_->d_[i].translator ();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QTranslator * Translate::qtTranslator (int i)
{
    if (uniq_ == NULL)
        return NULL;
    if ((i < 0) || (i >= uniq_->d_.count())) {
        return NULL;
    }
    return uniq_->d_[i].qtTranslator ();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void Translate::translatorDone (int i)
{
    if (uniq_ == NULL)
        return;
    if ((i < 0) || (i >= uniq_->d_.count())) {
        return;
    }
    return uniq_->d_[i].translatorDone ();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
int Translate::current()
{
    if (uniq_ == NULL)
        return -1;
    return uniq_->current_;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void Translate::setCurrent (int value)
{
    if (uniq_ == NULL) {
        return;
    } else if ((value < 0) || (value >= uniq_->d_.count())) {
        uniq_->current_ = -1;
    } else {
        uniq_->current_ = value;
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool Translate::sqliteAutoregister()
{
    bool b_ret = false;
# ifdef SQLITE3_FOUND
    int rc = sqlite3_auto_extension (getSqliteEntryPoint ());
    b_ret = (rc == SQLITE_OK);
#   else
    qCritical() << "sqlite-related functionality in translate pile "
                   "is disabled because sqlite3 "
                   "was not found at compile time";
# endif // SQLITE3_FOUND
    return b_ret;
}
/* ========================================================================= */

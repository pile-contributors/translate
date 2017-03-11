/**
 * @file translate.cc
 * @brief Definitions for translate class.
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2014 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#include "translang.h"
#include "translate.h"
#include "translate-private.h"

#include <QDir>

/**
 * @class TransLang
 *
 */

/* ------------------------------------------------------------------------- */
/**
 * This constructor is only useful for creating the invalid instance used by
 * the manager.
 */
TransLang::TransLang() :
    path_("/\\invalid instance;:"),
    langfile_("/\\invalid instance;:"),
    name_("/\\invalid instance;:"),
    icon_("/\\invalid instance;:"),
    locale_(),
    metadata_(),
    transl_(NULL),
    qttransl_(NULL)
{
    TRANSLATE_TRACE_ENTRY;

    TRANSLATE_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
/**
* ...
*/
TransLang::TransLang (
        const QString & dir, const QString & langfile,
        const QString & name, const QString & icon, const QString & locale) :
    path_(dir),
    langfile_(langfile),
    name_(name),
    icon_(icon),
    locale_(locale),
    metadata_(),
    transl_(NULL),
    qttransl_(NULL)
{
   TRANSLATE_TRACE_ENTRY;

   TRANSLATE_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
/**
 * ...
 */
TransLang::~TransLang()
{
    TRANSLATE_TRACE_ENTRY;

    TRANSLATE_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
/**
 * ...
 */
void TransLang::addMetadata (
        const QString & key, const QString & value)
{
    metadata_.insert (key, value);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
/**
 * ...
 */
bool TransLang::isValid () const
{
    return Translate::validInstance (*this);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QString TransLang::qtLangFile() const
{
    if (locale_.isEmpty())
        return QString();

    QDir dlang (path_);
    QString s_qt_name = QString("qt_%1.qm").arg(locale_);
    if (dlang.exists (s_qt_name)) {
        return dlang.absoluteFilePath (s_qt_name);
    } else {
        dlang.setPath (QString::fromUtf8 (getenv ("TRANSLATE_QT_DIR")));
        if (dlang.exists (s_qt_name)) {
            return dlang.absoluteFilePath (s_qt_name);
        } else {
            return QString();
        }
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QTranslator * TransLang::translator()
{
    if (transl_ != NULL)
        return transl_;
    transl_ = new QTranslator ();
    if (!transl_->load (langfile_)) {
        TRANSLATE_DEBUGM("Failed to load translator from file %s\n",
                         TMP_A(langfile_));
        delete transl_;
        transl_ = NULL;
    }

    return transl_;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
QTranslator * TransLang::qtTranslator()
{
    QString s_file = qtLangFile ();

    qttransl_ = new QTranslator ();
    if (!qttransl_->load (s_file)) {
        TRANSLATE_DEBUGM("Failed to load translator from file %s\n",
                         TMP_A(s_file));
        delete qttransl_;
        qttransl_ = NULL;
    }

    return qttransl_;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void TransLang::translatorDone ()
{
    if (transl_ != NULL) {
        delete transl_;
        transl_ = NULL;
    }
    if (qttransl_ != NULL) {
        delete qttransl_;
        qttransl_ = NULL;
    }
}
/* ========================================================================= */

void TransLang::anchorVtable() const {}

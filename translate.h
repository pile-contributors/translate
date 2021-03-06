/**
 * @file translate.h
 * @brief Declarations for translate class
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2014 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#ifndef GUARD_TRANSLATE_H_INCLUDE
#define GUARD_TRANSLATE_H_INCLUDE

#include <translate/translate-config.h>
#include <translate/translang.h>
#include <QVector>

//! Translation management class.
class TRANSLATE_EXPORT Translate {

public:

    //! Describes the entry point for sqlite.
    typedef void(*SqliteEntryPoint)(void);


private:

    //! Default constructor.
    Translate ();

    //! Destructor.
    virtual ~Translate();

public:

    //! Initialize the manager.
    static bool
    init (
            const char *env_var_path,
            QString * error);

    //! Terminate the manager.
    static void
    end ();

    //! Get the number of languages we know about.
    static int
    count () {
        if (uniq_ == NULL)
            return 0;
        return uniq_->d_.count();
    }

    //! Get a language at a particular index.
    static const TransLang &
    item (
            int i);

    //! Get a language given its name.
    static const TransLang &
    item (
            const QString & s_name);

    //! Just find the index (-1 if not found).
    static int
    itemIndex (
            const QString & s_name);

    //! Find the index by using the locale instead of name (-1 if not found).
    static int
    itemIndexFromLocale (
            const QString & s_locale);

    //! Tell if an instance is valid or not.
    static bool
    validInstance (
            const TransLang & item);

    //! Create a Qt translator.
    static QTranslator *
    translator (
            int i);

    //! Create a Qt translator.
    static QTranslator *
    qtTranslator (
            int i);

    //! Release cached translator.
    static void
    translatorDone (
            int i);

    //! Index of current translator.
    static int
    current ();

    //! Set the index of current translator.
    static void
    setCurrent (int value);

    //! The entry point for the sqlite3.
    static SqliteEntryPoint
    getSqliteEntryPoint ();

    //! Autoregister this extension with each new database
    //! (when not using the plugin).
    static bool
    sqliteAutoregister ();

protected:

private:
    QVector<TransLang> d_;
    int current_;
    static Translate * uniq_;
};

#endif // GUARD_TRANSLATE_H_INCLUDE

/**
 * @file translate.h
 * @brief Declarations for translate class
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2014 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#ifndef GUARD_TRANSLANG_H_INCLUDE
#define GUARD_TRANSLANG_H_INCLUDE

#include <translate/translate-config.h>
#include <QString>
#include <QMap>
#include <QTranslator>

//! One language provided by the translator.
class TRANSLATE_EXPORT TransLang {

    friend class Translate;

public:

    //! Constructor that creates a valid instance.
    explicit TransLang (
            const QString & dir,
            const QString & langfile,
            const QString & name,
            const QString & icon,
            const QString & locale);

    //! Default constructor that creates an invalid instance.
    TransLang ();

    //! Destructor.
    virtual ~TransLang();

    //! assignment operator
    TransLang& operator=( const TransLang& other) {
        path_ = other.path_;
        langfile_ = other.langfile_;
        name_ = other.name_;
        icon_ = other.icon_;
        locale_ = other.locale_;
        metadata_ = other.metadata_;
        transl_ = other.transl_;
        qttransl_ = other.qttransl_;
        return *this;
    }

    //! Tell if this instance is valid or not.
    bool
    isValid () const;

    //! Absolute paths towards language directory.
    const QString &
    langDirectory () const {
        return path_;
    }

    //! Get the absolute path for the language file.
    const QString &
    langFile () const {
        return langfile_;
    }

    //! Get the absolute path for the qt language file (if any).
    QString
    qtLangFile () const;

    //! Get the name of this language.
    const QString &
    langName () const {
        return name_;
    }

    //! Get the name of this locale.
    const QString &
    localeName () const {
        return locale_;
    }

    //! Get the icon of this language.
    const QString &
    langIcon () const {
        return icon_;
    }

    //! Get the icon of this language.
    const QMap<QString, QString> &
    metadata () const {
        return metadata_;
    }

    bool
    hasCachedTranslator () const {
        return transl_ != NULL;
    }

    bool
    hasCachedQtTranslator () const {
        return qttransl_ != NULL;
    }

protected:

    //! Create a Qt translator.
    QTranslator *
    translator ();

    //! Create a Qt translator.
    QTranslator *
    qtTranslator ();

    //! Discard cached copy of the translator.
    void
    translatorDone ();

    //! Insert some metadata.
    void
    addMetadata (
            const QString & key,
            const QString & value);


private:
    QString path_;
    QString langfile_;
    QString name_;
    QString icon_;
    QString locale_;
    QMap<QString, QString> metadata_;
    QTranslator * transl_;
    QTranslator * qttransl_;

public: virtual void anchorVtable() const;
};

inline bool operator== (
        const TransLang& lhs, const TransLang& rhs) {
    return (lhs.langDirectory () == rhs.langDirectory ()) &&\
           (lhs.langName () == rhs.langName ()) &&\
           (lhs.langIcon () == rhs.langIcon ()) &&\
           (lhs.langFile () == rhs.langFile ());
}
inline bool operator!= (
        const TransLang& lhs, const TransLang& rhs) {
    return !operator==(lhs, rhs);
}

#endif // GUARD_TRANSLANG_H_INCLUDE

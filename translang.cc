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

/**
 * @class translate
 *
 * Detailed description.
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
    transl_(NULL)
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
    transl_(NULL)
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
QTranslator * TransLang::translator()
{
    if (transl_ != NULL)
        return transl_;
    transl_ = new QTranslator ();
    if (!transl_->load (langfile_)) {
        TRANSLATE_DEBUGM("Failed to load translator from file");
        delete transl_;
        transl_ = NULL;
    }

    return transl_;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void TransLang::translatorDone ()
{
    if (transl_ != NULL) {
        delete transl_;
        transl_ = NULL;
    }
}
/* ========================================================================= */



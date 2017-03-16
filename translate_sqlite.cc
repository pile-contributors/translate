/**
 * @file translate_sqlite.cc
 * @brief Definitions for translate class.
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2014 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#include "translate.h"
#include "translate-private.h"
#ifdef SQLITE3_FOUND

#include <sqlite/sqlite3ext.h>
static SQLITE_EXTENSION_INIT1

#include <QCoreApplication>
#include <QDebug>

#define TR_FUNC_NAME "tr"


/* ------------------------------------------------------------------------- */
//! Helper for retrieving strings from values.
QString value2string (void * val)
{
    sqlite3_value *value = static_cast<sqlite3_value *>(val);

    return QString(reinterpret_cast<const QChar *>(
                       sqlite3_value_text16 (value)),
                       sqlite3_value_bytes16 (value) / sizeof(QChar));
}
/* ========================================================================= */

extern "C" {

/* ------------------------------------------------------------------------- */
//! Implementation of the `translate` function.
static void epoint_tr (
            sqlite3_context *context, int argc, sqlite3_value **argv)
{
    TRANSLATE_TRACE_ENTRY;
    bool b_ret = false;

    for (;;) {
        if (argc != 1) {
            sqlite3_result_error (
                        context,
                        TR_FUNC_NAME " takes one argument: "
                                     "the string to translate", -1);
            sqlite3_result_error_code (context, SQLITE_MISUSE);
            break;
        }

        QString result = QCoreApplication::translate (
                    "sqlite", (const char *)sqlite3_value_text(argv[0]));
        sqlite3_result_blob(
                    context, result.toUtf8().constData(), -1, SQLITE_TRANSIENT);

        b_ret = true;
        break;
    }
    TRANSLATE_TRACE_EXIT;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
//! The entry point used by sqlite.
TRANSLATE_EXPORT int sqlite3_translatepile_init (
            sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi)
{
    TRANSLATE_TRACE_ENTRY;
    int rc = SQLITE_ERROR;
    for (;;) {
        SQLITE_EXTENSION_INIT2(pApi);

        rc = sqlite3_create_function_v2 (
                    db,
                    /* zFunctionName */ "tr",
                    /* nArg */ 1,
                    /* eTextRep */ SQLITE_UTF8 | SQLITE_DETERMINISTIC,
                    /* pApp */ NULL,
                    /* xFunc */ epoint_tr,
                    /* xStep */ NULL,
                    /* xFinal */ NULL,
                    /* xDestroy */ NULL);
        if (rc != SQLITE_OK) {
            *pzErrMsg = sqlite3_mprintf (
                        "Failed to register translation function: %s\n",
                        sqlite3_errmsg(db));
            break;
        }

        rc = SQLITE_OK;
        break;
    }
    TRANSLATE_TRACE_EXIT;
    return rc;
}
/* ========================================================================= */

} // extern "C"
#endif // SQLITE3_FOUND

/* ------------------------------------------------------------------------- */
Translate::SqliteEntryPoint Translate::getSqliteEntryPoint ()
{
    Translate::SqliteEntryPoint result = NULL;
#   ifdef SQLITE3_FOUND
    result = (void(*)(void))sqlite3_translatepile_init;
#   else
    qCritical() << "sqlite-related functionality in translate pile "
                   "is disabled because sqlite3 "
                   "was not found at compile time";
#   endif // SQLITE3_FOUND
    return result;
}
/* ========================================================================= */

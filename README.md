Translate
=========

This pile provides support for an application that
wants to deal with multiple languages and chooses to
use Qt5 internally and for translation management.

The application is expected to create a directory to hold
language-dependent files (say `internationalization`)
that contains one folder for each language and the
CMakeLists.txt file calls `internationalizationDirectory()` macro.

The macro will search for .ts files and run them through Qt system
and will install generated .qm files in `share/locales/${project}`
along with the other components.

Each language directory needs to have at least one
file called `metadata.ini`.

    [General]
    label=English
    icon=icon.png
    translate=strings-en
    locale=en_GB
    additional=a.txt;b.txt;c.txt;d.txt

The format is supported by Qt's QSettings and is also parsed
by CMake script in order to extract meaningful values. For this
reason  the format has a few limitations:

    - no white spaces should appear on either side of equal (=) sign
    - semicolon (;) character may cause problems

Other keys may appear in this file but only these are meaningful
for this pile.

`label` is the text that will be presented to the user and should
be a (short) name of the language.

`icon` is the image to be presented in GUI applications along
with the label;

`translate` is both the name of the .ts file located in same
directory as the `metadata.ini` file as well as the name of the
compiled `.qm` file that will be generated and installed. The
`.ts` file does not need to exist but it will be created by the
script in *source* directory. One thing to keep in mind is that
the `.qm` files are generated in same directory and, thus, the name
must be unique across all language files.

`additional` is the list of additional files to be installed in
language directory. The list should be separated by semicolons (;).

The CMake script creates an additional target called
`translate` that is always builded.

Files are installed in either
`${PROJECT_NAME}.app/Contents/Resources/translations/${lang}` for MacOs
or in `share/locale/${PROJECT_NAME_UNIX}/${lang}` for the rest.

`Translate` class acts as a manager that is capable of locating, reading
and storing the languages. Each language is represented as a `TransLang`.
Code can then iterate and set current language.



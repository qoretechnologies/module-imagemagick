RPM packaging
=============

``qore-imagemagick-module.spec`` builds the native ImageMagick 7 bindings,
compiled ImageMagickDataProvider, matching source modules, provider resources,
translations, metadata and ``qimage``. API documentation is a separate noarch
package. The Qore RPM helpers preserve AOT dependency trailers during stripping.

The recipe requires the installed Qore 3 SDK, MagickWand 7 or later and fonts
for text rendering. Tests, complete standard-locale catalog validation, CLI
image conversion and strict Doxygen checks are enabled by default. An
ImageMagick 6-only distribution needs a compatible ImageMagick 7 dependency
before this module can be built. No major-version fallback is permitted.

Prepare a pinned source bundle with ``qore-packaging/tools/packaging.py`` and
build it unprivileged using ``qore-packaging/tools/build-local.py``. To test an
installed package without the SDK or compiler, install ``diffutils`` for the
CLI input-preservation check and run from the extracted source::

    QORE_RPM_TEST_TMP=/tmp/imagemagick-installed rpm/tests-installed-runtime

The fixture copies only tests outside the checkout and uses installed module
paths and the installed qimage command. It exercises image transformations,
annotations and in-place conversion, while checking the original input remains
unchanged when a distinct output is requested.

``python3 -B -W error test/test_docs.py build -v`` checks native and provider API
pages. ``python3 -B -W error test/test_uninstall.py -v`` checks staged uninstall,
paths with spaces, dangling symlinks, repeated removal and rejection of missing
manifests and directory entries. Runtime feature availability also depends on
the distribution's ImageMagick codec and security policy packages.

RPM lint reports the local source archive and hidden catalog ownership manifest.
The latter is required by Qore catalog reconciliation and belongs in the package.

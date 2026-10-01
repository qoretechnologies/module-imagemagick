#!/usr/bin/python3
# Copyright (C) 2026 Qore Technologies, s.r.o.
# SPDX-License-Identifier: MIT
"""Verify public native image and provider API pages."""
from pathlib import Path
import sys
import unittest
import xml.etree.ElementTree as ET

BUILD = Path(sys.argv.pop(1)).resolve()


class DocumentationTests(unittest.TestCase):
    def test_public_image_and_drawing_classes_are_documented(self):
        tree = ET.parse(BUILD / "imagemagick.tag")
        names = {c.findtext("name"): c for c in tree.findall("compound")}
        for name in ("Qore::ImageMagick::MagickImage", "Qore::ImageMagick::MagickDrawing"):
            with self.subTest(name=name):
                self.assertIn(name, names)
                self.assertTrue((BUILD / "docs/imagemagick/html" / names[name].findtext("filename")).is_file())
        self.assertFalse(any("QoreMagickImage" in name for name in names), "private C++ wrapper is not public API")

    def test_public_provider_and_request_types_have_pages(self):
        tree = ET.parse(BUILD / "ImageMagickDataProvider.tag")
        names = {c.findtext("name"): c for c in tree.findall("compound")}
        for name in ("ImageMagickResizeDataProvider", "ImageMagickResizeRequestDataType",
                     "ImageMagickAnnotateRequestDataType"):
            with self.subTest(name=name):
                full = "ImageMagickDataProvider::" + name
                self.assertIn(full, names)
                self.assertTrue((BUILD / "docs/ImageMagickDataProvider/html"
                                 / names[full].findtext("filename")).is_file())


if __name__ == "__main__":
    unittest.main()

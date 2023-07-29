# Version history of webp-viewer

## Version 0.5.0-pre (2023-07-30)

The viewer can now handle multiple files. A user can now pass paths to multiple
image files as arguments, and the viewer will allow to cycle through those files
with the <kbd>D</kbd> or <kbd>&#8594;</kbd> key and <kbd>A</kbd> or
<kbd>&#8592;</kbd> key, respectively.

## Version 0.4.3 (2022-08-19)

The viewer will now emit a warning on the standard output, if the size of the
monitor cannot be determined. As before, it will then continue to assume the
default size of 640 x 480 pixels.

## Version 0.4.2 (2022-06-15)

WebP images with alpha channel will now be shown with alpha channel data.

## Version 0.4.1 (2022-03-26)

If the displayed image was scaled down to fit into the windows, then the scaling
factor is now shown in the title bar of the window.

## Version 0.4.0 (2022-03-26)

Add hint about missing support for animations when the user attempts to load a
WebP image containing animations.

## Version 0.3.0 (2022-03-25)

The windows size is now adjusted according to the image size so that the image
ratio is respected.

## Version 0.2.0 (2022-03-25)

The image is now actually shown in the window, replacing the placeholder
triangle from previous versions.

This is the first version that actually can display image data.

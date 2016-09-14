# Tag Parser
C++ library for reading and writing MP4 (iTunes), ID3, Vorbis, Opus, FLAC and Matroska tags.

## Supported formats
The tag library can read and write the following tag formats:
- iTunes-style MP4/M4A tags (MP4-DASH is supported)
- ID3v1 and ID3v2 tags
  - conversion between ID3v1 and different versions of ID3v2 is possible
- Vorbis, Opus and FLAC comments in Ogg streams
  - cover art via "METADATA_BLOCK_PICTURE" is supported
- Vorbis comments and "METADATA_BLOCK_PICTURE" in raw FLAC streams
- Matroska/WebM tags and attachments

## File layout options
### Tag position
The library allows you to choose whether tags should be placed at the beginning or at
the end of an MP4/Matroska file.

### Padding
Padding allows adding additional tag information without rewriting the entire file
or appending the tag. Usage of padding can be configured:
- minimum/maximum padding: The file is rewritten if the padding would fall below/exceed the specifed limits.
- preferred padding: If the file needs to be rewritten the preferred padding is used.

However, it is also possible to force rewriting the entire file.

Taking advantage of padding is currently not supported when dealing with Ogg streams (it is supported when dealing with raw FLAC streams).

## Additional features
The library can also display technical information such as the ID, format, language, bitrate,
duration, size, timestamps, sampling frequency, FPS and other information of the tracks.

It also allows to inspect and validate the element structure of MP4 and Matroska files.

## Text encoding, Unicode support
The library is aware of different text encodings and can convert between different encodings using iconv.

## Usage
For examples check out the command line interface of [Tag Editor](https://github.com/Martchus/tageditor).
API documentation can be generated using Doxygen with `make tagparser_apidoc`.

## Bugs, stability
- Matroska files composed of more than one segment aren't tested yet and might not work
- It is recommend you to create backups before editing because I can not test whether the library works with all kind of files

## Build instructions
The tagparser library depends on c++utilities and is built in the same way.
It also depends on zlib.

## TODO
- Support more formats (EXIF, PDF metadata, Theora, ...)
- Do tests with Matroska files which have multiple segments
- Support adding cue-sheet to FLAC files

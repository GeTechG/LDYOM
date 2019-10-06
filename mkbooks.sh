#!/bin/bash

# Copyright (c) 2018 Patrick Pedersen <ctx.xda@gmail.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

# Print usage
function usage {
  echo "mkbooks.sh [-h] <cfg> <out> [extra]"
  echo
  echo "A simple script that exports a mkdocs directory to a PDF or EPUB"
  echo
  echo "Arguments:"
  echo
  echo -e "  -h\t\tDisplay this usage"
  echo -e "  cfg\t\tPath to the Mkdocs yml configuration file"
  echo -e "  out\t\tName and type of the output file (Example: Docs.pdf or Docs.epub)"
  echo -e "  extra\t\tAdditional arguments that are passed to mdpdf or pandoc (pdf = mdpdf, pandoc = epub). Use quotation marks!"
  echo
}

# -h provided, print usage
if [ "$1" == "-h" ]; then
  usage
  exit 1
fi

# Check if enough parameters have been provided
if [ -z "$2" ]; then
  echo "Missing parameters!"
  echo
  usage
  exit -1
fi

## Check validity of configuration path

# Check if cfg exists
if [ ! -f "$1" ]; then
  echo "mkbooks: No such file: '$1'"
  exit -1
fi

# Check if cfg ends with .yml
if [[ "${1##*.}" !=  "yml" ]]; then
  echo "mkbooks: '$1' does not appear to be a mkdocs configuration file!"
  exit -1
fi

## Check validity of output

# Check if output file is pdf or epub
if [[ "${2##*.}" == "pdf" ]]; then
  IS_PDF=1
elif [[ "${2##*.}" == "epub" ]]; then
  IS_PDF=0
else
  echo "mkbooks: Output file does not seem to be a pdf or epub!"
  exit -1
fi

## Check dependencies

# for mdpdf
if ! mdpdf -v foo >/dev/null 2>&1; then
  echo "mkbooks: mdpdf has not been installed!"
  exit -1
fi

# for pandoc
if ! pandoc -v foo >/dev/null 2>&1; then
  echo "mkbooks: pandoc has not been installed!"
  exit -1
fi

# for pdfunite
if ! pdfunite -v foo >/dev/null 2>&1; then
  echo "mkbooks: pdfunite is not available on this system!"
  exit -1
fi

# Flags and itterators
READING_PAGES=0
I=0
DOCS=() # Required for pandoc

# Read mkdocs configuration
while read line; do

  # Read 'pages:' section!
  if [ "$READING_PAGES" -gt 0 ]; then

    # No longer part of "pages:" section!
    if [[ ! -z "$line" && ! $line =~ ^.*-.*$ ]]; then
      break
    fi

    # Process line
    if [[ "$line" =~ \-.*:\ .* ]]; then

      # Get path of md file
      MD_PATH="$(dirname $1)/docs/$(echo "$line" | grep -oP "(?<=\: ).*")"

      # Path not found!
      if [ ! -f "$MD_PATH" ]; then
        echo "mkbooks: '$MD_PATH' not found!"
        rm -R  "/tmp/mkbooks/"
        exit -1
      fi

      # Export to pdf
      if [ "$IS_PDF" -gt 0 ]; then
        if ! mdpdf  "$MD_PATH" "/tmp/mkbooks/$I.pdf" "$3"; then
          echo "Converting failed!"
          exit -1
        fi

      # Add to pandoc input arg list
      else
        DOCS+=("$(echo "$line" | grep -oP "(?<=\: ).*")")
      fi

      ((I++))
    fi

  # 'pages:' found!
  elif [[ "$line" == "pages:" ]]; then
    READING_PAGES=1
    rm -R -f /tmp/mkbooks
    mkdir /tmp/mkbooks # Create temporary output directory
  fi
done < "$1"

if [[ "$READING_PAGES" > 0 && "$I" > 0 ]]; then

  # Merge PDFs
  if [ "$IS_PDF" -gt 0 ]; then
    pdfunite /tmp/mkbooks/* "$2"

  # Generate EPUB
  else
    if [[ ${2:0:1} == '/' ]]; then
      OUT="$2"
    else
      OUT="$PWD/$2"
    fi

    # Dive into docs/ directory and convert md to EPUB using pandoc!
    if [ -z "$3" ]; then
      cd $(dirname $1)/docs && pandoc "${DOCS[@]}" -o "$OUT";
    else
      cd $(dirname $1)/docs && pandoc "$3" "${DOCS[@]}" -o "$OUT";
    fi

    if [ ! -f "$OUT" ]; then
      echo "Converting failed!"
      exit -1
    fi
  fi

  echo "Successfully exported docs to $( if [ $IS_PDF -gt 0 ]; then echo "PDF"; else echo "EPUB"; fi )!"
  rm -R /tmp/mkbooks
else
  echo "No pages specified in mkdocs configuration!"
fi

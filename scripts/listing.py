#!/usr/bin/python

import os.path
import fnmatch
import sys

latex_file = """
\\documentclass{{article}}

\\usepackage{{subfiles}}
\\usepackage{{fullpage}}
\\usepackage{{caption}}
\\usepackage[final]{{pdfpages}}
\\usepackage{{lmodern}}
\\usepackage{{color}}
\\usepackage{{listings}}
\\usepackage{{fancyhdr}}
\\pagestyle{{fancy}}

\\lhead{{ J. Anthony Sterrett, Jr. // Andrew Brinker }}
\\rhead{{ Operating Systems Lab {LABNO:d}; CSE-460 }}
\\renewcommand{{\\headsep}}{{25pt}}

\\lstset{{
  language=C++,
  tabsize=4,
  breaklines=true,
  showstringspaces=false,
  basicstyle=\\ttfamily\\footnotesize
}}

\\title{{ Operating Systems Lab {LABNO:d}; CSE-460 }}
\\author{{ J. Anthony Sterrett, Jr. \\\\ Andrew Brinker }}
\\date{{}}

\\begin{{document}}

\\pagenumbering{{Alph}}
\\begin{{titlepage}}
\\maketitle
\\thispagestyle{{empty}}
\\end{{titlepage}}
\\pagenumbering{{arabic}}

\\lstinputlisting[title=Makefile]{{Makefile}}
\\clearpage
{LISTINGS}

\\end{{document}}
"""

listing_line = """\\lstinputlisting[title={FILENAME}]{{{FILEPATH}}}
\\clearpage

"""

bin_name = ""

def make_listing():
  d = int( raw_input( "Enter lab number: " ) )
  listings = ""
  headers = {}
  sources = {}

  for root, dirnames, filenames in os.walk( 'src' ):
    for filename in fnmatch.filter( filenames, '*.cpp' ):
      sources[filename] = os.path.join( root, filename )
    for filename in fnmatch.filter( filenames, '*.h' ):
      headers[filename] = os.path.join( root, filename )
  
  header_filenames = sorted( headers.keys() )
  source_filenames = sorted( sources.keys() )

  while header_filenames and source_filenames:
    h = header_filenames[0]
    s = source_filenames[0]

    if os.path.splitext( h )[0] == os.path.splitext( s )[0]:
      listings += listing_line.format( FILENAME = h, FILEPATH = headers[h] )
      listings += listing_line.format( FILENAME = s, FILEPATH = sources[s] )
      del header_filenames[0]
      del source_filenames[0]
    else:
      if h < s:
        listings += listing_line.format( FILENAME = h, FILEPATH = headers[h] )
        del header_filenames[0]
      else:
        listings += listing_line.format( FILENAME = h, FILEPATH = sources[s] )
        del source_filenames[0]

  for h in header_filenames:
    listings += listing_line.format( FILENAME = h, FILEPATH = headers[h] )
  
  for s in source_filenames:
    listings += listing_line.format( FILENAME = s, FILEPATH = sources[s] )

  with open( "{}.tex".format( bin_name ), "w" ) as f: 
    f.write( latex_file.format( LABNO = d, LISTINGS = listings ) )  

if __name__ == "__main__":
  bin_name = sys.argv[1]
  make_listing()

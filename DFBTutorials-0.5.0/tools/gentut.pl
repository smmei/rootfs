#!/usr/bin/perl
#
#   (c) Copyright 2000  convergence integrated media GmbH.
#   (c) Copyright 2002  convergence GmbH.
#   All rights reserved.
#
#   Written by Denis Oliver Kropp <dok@directfb.org>,
#              Andreas Hundt <andi@fischlustig.de> and
#              Sven Neumann <neo@directfb.org>
#
#
#
#   This file is subject to the terms and conditions of the MIT License:
#
#   Permission is hereby granted, free of charge, to any person
#   obtaining a copy of this software and associated documentation
#   files (the "Software"), to deal in the Software without restriction,
#   including without limitation the rights to use, copy, modify, merge,
#   publish, distribute, sublicense, and/or sell copies of the Software,
#   and to permit persons to whom the Software is furnished to do so,
#   subject to the following conditions:
#
#   The above copyright notice and this permission notice shall be
#   included in all copies or substantial portions of the Software.
#
#   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
#   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
#   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
#   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
#   CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
#   TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
#   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


################################################################
#                                                              #
#  Tutorial generator (early stage of implementation)          #
#                                                              #
#  - Reads tutorials from stdin                                #
#  - Writes HTML to several files                              #
#                                                              #
################################################################



html_create( INDEX, "html/index.html", "DirectFB Tutorials" );

$mode = 0;

$tutorial = "";
$comment = "";
$code = "";

print INDEX "<P>\n",
            "  <TABLE border=0>\n";

while (<>)
   {
      chomp;

      if ($mode == 0)
         {
            if ( /^\s*\/\*\*\s*$/ )
               {
                  flush_code_comment();

                  if ($tutorial)
                     {
                        print TUTORIAL "  </TABLE>\n",
                                       "</P>\n";
                        html_close( TUTORIAL );
                     }

                  $tutorial = "";

                  while (<>)
                     {
                        chomp;

                        if ( /^\s*\*\/\s*$/ )
                           {
                              last;
                           }
                        elsif ( /^\s*\*\s*(.+)\.c$/ )
                           {
                              $filename = $1;
                           }
			elsif ( /^\s*\*\s*(.+)$/ )
			   {
			       $tutorial = $1;
			   }
                     }

                  if ($tutorial && $filename)
                     {
		        print INDEX "<tr><td>",
		                    "<a href=\"$filename.html\">$tutorial</a>",
			            "</td></tr>\n";

                        html_create( TUTORIAL,
				     "html/$filename.html", "$tutorial" );
                        print TUTORIAL "<P>\n",
                                       "  <TABLE border=0>\n";
                     }
                  else
                     {
                        die "Could not parse tutorial header";
                     }
               }
            elsif ( /^\s*\/\*\s*$/ )
               {
                  flush_code_comment();

                  $mode = 1;
               }
            elsif ( /^(\s*)(\S.*)$/ )
               {
                  $line = "$1$2";
                  $line =~ s/\</&\#60\;/g;
                  $line =~ s/\>/&\#62\;/g;

                  $code .= "$line<br>";
               }
            else
               {
                  flush_code_comment();
               }
         }
      elsif ($mode == 1)
         {
            if ( /^\s*\*\/\s*$/ )
               {
                  $mode = 0;
               }
            elsif ( /^\s*\*(.+)$/ )
               {
                  $comment .= "$1<br>";
               }
         }
   }

flush_code_comment();

if ($tutorial)
   {
      print TUTORIAL "  </TABLE>\n",
                     "</P>\n";
      html_close( TUTORIAL );
   }

print INDEX "  </TABLE></CENTER>\n",
            "</P>\n";

html_close( INDEX );



sub flush_code_comment ()
   {
      if ($code && $tutorial)
         {
            print TUTORIAL "<tr valign=top><td bgcolor=#EEEEEE>\n";
            print TUTORIAL "<font color=\"#303030\"><pre>$code</pre></font>\n";
            print TUTORIAL "</td><td bgcolor=#DDEEDD><small>\n";

            if ($comment)
               {
                  print TUTORIAL "<font color=#102000>$comment</font>\n";
               }
            else
               {
                  print TUTORIAL "&nbsp;\n";
               }

            print TUTORIAL "</small></td></tr>\n";
         }

      $comment = "";
      $code = "";
   }


sub html_create (FILEHANDLE, FILENAME, TITLE, SUBTITLE)
   {
      my $FILE = shift(@_);
      my $filename = shift(@_);
      my $title = shift(@_);
      my $subtitle = shift(@_);

      open( $FILE, ">$filename" ) || die;

      print $FILE "<HTML>\n",
                  "<STYLE>\n",
                  "  <!--\n",
                  "    A{textdecoration:none}\n",
                  "  -->\n",
                  "</STYLE>\n",
                  "<HEAD>\n",
                  "  <TITLE>DirectFB Tutorials</TITLE>\n",
                  "</HEAD>\n",
                  "<BODY bgcolor=#FFFFFF link=#0070FF",
                       " vlink=#0070FF text=#404040>\n",
                  "\n",
                  "<TABLE width=100% bgcolor=black border=0 cellspacing=1 cellpadding=3>\n",
                  "  <TR><TD width=30%>\n",
                  "    <A href=\"http://www.directfb.org\"><IMG border=0 src=\"directfb.png\"></A>\n",
                  "  </TD><TD align=right>\n",
                  "    &nbsp;&nbsp;",
                  "    <A href=\"index.html\"><FONT size=+3 color=white>DirectFB Tutorials</FONT></A>\n",
                  "  </TD></TR>\n",
                  "  <TR><TD colspan=2 align=center bgcolor=#303030>\n";

      if ($subtitle)
         {
            print $FILE "    <TABLE border=0 cellspacing=0 cellpadding=0>\n",
                        "      <TR><TD align=right width=50%>\n",
                        "        <BIG>$title&nbsp;</BIG>\n",
                        "      </TD><TD align=left width=50%>\n",
                        "        <BIG><FONT color=yellow>&nbsp;$subtitle</FONT></BIG>\n",
                        "      </TD></TR>\n",
                        "    </TABLE>\n";
         }
      else
         {
            print $FILE "    <BIG><FONT color=yellow>$title</FONT></BIG>\n";
         }

      print $FILE "  </TD></TR>\n",
                  "</TABLE>\n",
                  "\n";
   }

sub html_close (FILEHANDLE)
   {
      my $FILE = shift(@_);

      print $FILE "\n",
                  "<TABLE width=100% bgcolor=black border=0 cellspacing=0 cellpadding=0>\n",
                  "  <TR><TD valign=center>\n",
                  "    <FONT size=-3>&nbsp;&nbsp;(C) Copyright by convergence GmbH</FONT>\n",
                  "  </TD><TD align=right>\n",
                  "    <A href=\"http://www.convergence.de\">\n",
                  "      <IMG border=0 hspace=4 src=\"cimlogo.png\">\n",
                  "    </A>\n",
                  "  </TD></TR>\n",
                  "</TABLE>\n",
                  "\n",
                  "</BODY>\n",
                  "</HTML>\n";

      close( $FILE );
   }


/* Install ImageMagick to convert image_file to pixmaps file
typing the command: convert file.png file.xpm  
*/

#ifndef PIXMAPS_H
#define PIXMAPS_H

static const char *print_xpm[] =
{
    "32 32 12 1",
    "a c #ffffff",
    "h c #ffff00",
    "c c #ffffff",
    "f c #dcdcdc",
    "b c #c0c0c0",
    "j c #a0a0a4",
    "e c #808080",
    "g c #808000",
    "d c #585858",
    "i c #00ff00",
    "# c #000000",
    ". c None",
    "................................",
    "................................",
    "...........###..................",
    "..........#abb###...............",
    ".........#aabbbbb###............",
    ".........#ddaaabbbbb###.........",
    "........#ddddddaaabbbbb###......",
    ".......#deffddddddaaabbbbb###...",
    "......#deaaabbbddddddaaabbbbb###",
    ".....#deaaaaaaabbbddddddaaabbbb#",
    "....#deaaabbbaaaa#ddedddfggaaad#",
    "...#deaaaaaaaaaa#ddeeeeafgggfdd#",
    "..#deaaabbbaaaa#ddeeeeabbbbgfdd#",
    ".#deeefaaaaaaa#ddeeeeabbhhbbadd#",
    "#aabbbeeefaaa#ddeeeeabbbbbbaddd#",
    "#bbaaabbbeee#ddeeeeabbiibbadddd#",
    "#bbbbbaaabbbeeeeeeabbbbbbaddddd#",
    "#bjbbbbbbaaabbbbeabbbbbbadddddd#",
    "#bjjjjbbbbbbaaaeabbbbbbaddddddd#",
    "#bjaaajjjbbbbbbaaabbbbadddddddd#",
    "#bbbbbaaajjjbbbbbbaaaaddddddddd#",
    "#bjbbbbbbaaajjjbbbbbbddddddddd#.",
    "#bjjjjbbbbbbaaajjjbbbdddddddd#..",
    "#bjaaajjjbbbbbbjaajjbddddddd#...",
    "#bbbbbaaajjjbbbjbbaabdddddd#....",
    "###bbbbbbaaajjjjbbbbbddddd#.....",
    "...###bbbbbbaaajbbbbbdddd#......",
    "......###bbbbbbjbbbbbddd#.......",
    ".........###bbbbbbbbbdd#........",
    "............###bbbbbbd#.........",
    "...............###bbb#..........",
    "..................###..........."
};


static const char *zoom_xpm[] =
{
    "32 32 8 1",
    "# c #000000",
    "b c #c0c0c0",
    "a c #ffffff",
    "e c #585858",
    "d c #a0a0a4",
    "c c #0000ff",
    "f c #00ffff",
    ". c None",
    "..######################........",
    ".#a#baaaaaaaaaaaaaaaaaa#........",
    "#aa#baaaaaaaaaaaaaccaca#........",
    "####baaaaaaaaaaaaaaaaca####.....",
    "#bbbbaaaaaaaaaaaacccaaa#da#.....",
    "#aaaaaaaaaaaaaaaacccaca#da#.....",
    "#aaaaaaaaaaaaaaaaaccaca#da#.....",
    "#aaaaaaaaaabe###ebaaaaa#da#.....",
    "#aaaaaaaaa#########aaaa#da#.....",
    "#aaaaaaaa###dbbbb###aaa#da#.....",
    "#aaaaaaa###aaaaffb###aa#da#.....",
    "#aaaaaab##aaccaaafb##ba#da#.....",
    "#aaaaaae#daaccaccaad#ea#da#.....",
    "#aaaaaa##aaaaaaccaab##a#da#.....",
    "#aaaaaa##aacccaaaaab##a#da#.....",
    "#aaaaaa##aaccccaccab##a#da#.....",
    "#aaaaaae#daccccaccad#ea#da#.....",
    "#aaaaaab##aacccaaaa##da#da#.....",
    "#aaccacd###aaaaaaa###da#da#.....",
    "#aaaaacad###daaad#####a#da#.....",
    "#acccaaaad##########da##da#.....",
    "#acccacaaadde###edd#eda#da#.....",
    "#aaccacaaaabdddddbdd#eda#a#.....",
    "#aaaaaaaaaaaaaaaaaadd#eda##.....",
    "#aaaaaaaaaaaaaaaaaaadd#eda#.....",
    "#aaaaaaaccacaaaaaaaaadd#eda#....",
    "#aaaaaaaaaacaaaaaaaaaad##eda#...",
    "#aaaaaacccaaaaaaaaaaaaa#d#eda#..",
    "########################dd#eda#.",
    "...#dddddddddddddddddddddd##eda#",
    "...#aaaaaaaaaaaaaaaaaaaaaa#.####",
    "...########################..##."
};

/* XPM */
static const char *run_xpm[] = {
/* columns rows colors chars-per-pixel */
"48 48 110 2 ",
"   c #07671F",
".  c #066C1B",
"X  c #0B7C17",
"o  c #07711A",
"O  c #097419",
"+  c #0A7918",
"@  c #147E1B",
"#  c #0A6922",
"$  c #0F6829",
"%  c #106E27",
"&  c #146E2E",
"*  c #127625",
"=  c #127A23",
"-  c #14732A",
";  c #1B752B",
":  c #1D6934",
">  c #1D7337",
",  c #236D3B",
"<  c #28753A",
"1  c #227A39",
"2  c #2B7746",
"3  c #297B42",
"4  c #307E48",
"5  c #3E7F63",
"6  c #407F66",
"7  c #139E0F",
"8  c #0D8415",
"9  c #0E8A13",
"0  c #108F12",
"q  c #13831C",
"w  c #158A1C",
"e  c #119311",
"r  c #139B11",
"t  c #159119",
"y  c #1B961F",
"u  c #17981A",
"i  c #1B9B1E",
"p  c #16A211",
"a  c #19A614",
"s  c #1AA916",
"d  c #1EA31F",
"f  c #1DAC19",
"g  c #1FB01C",
"h  c #21B21D",
"j  c #1F8426",
"k  c #1C8729",
"l  c #1D8B28",
"z  c #1D9F21",
"x  c #24832D",
"c  c #28872F",
"v  c #25932C",
"b  c #2A9F2E",
"n  c #248434",
"m  c #29843C",
"M  c #2D883F",
"N  c #308D3F",
"B  c #30953A",
"V  c #349B3A",
"C  c #1DA021",
"Z  c #1FA820",
"A  c #21AB21",
"S  c #2CA42D",
"D  c #23B420",
"F  c #2AB528",
"G  c #30A431",
"H  c #35AA35",
"J  c #39AE36",
"K  c #39A53C",
"L  c #39B433",
"P  c #3EB63E",
"I  c #338446",
"U  c #318944",
"Y  c #34834D",
"T  c #358A49",
"R  c #398D4D",
"E  c #379346",
"W  c #389645",
"Q  c #3B9C44",
"!  c #3B944A",
"~  c #3B9848",
"^  c #378252",
"/  c #3B8555",
"(  c #3B8953",
")  c #3D835C",
"_  c #3AA141",
"`  c #3E8061",
"'  c #408F55",
"]  c #438B5E",
"[  c #48915F",
"{  c #43A74B",
"}  c #4EA25D",
"|  c #50A65F",
" . c #52AA5F",
".. c #51BA55",
"X. c #58B95F",
"o. c #438366",
"O. c #478965",
"+. c #498D65",
"@. c #4D9265",
"#. c #4E9A63",
"$. c #509C63",
"%. c #52A263",
"&. c #54AB61",
"*. c #57B261",
"=. c #5ABC60",
"-. c #4CC24B",
";. c #55C655",
":. c #58C956",
">. c #5EC85F",
",. c None",
/* pixels */
",.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.,.S G j ,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.S L J H v ,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.G J K K Q B ; ,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.G K Q Q W W E m ,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.V Q ~ E ! R R ( T : ,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.B ~ ! R R / ( / ) ( 3 ,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.B ! R R / ( / ) ) ) ` ^ : ,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.N R ( ( ( / ) ` 5 5 5 o.) 2 ,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.U ( ( / ) 5 ` 5 5 5 6 6 o.o./ , ,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.T / / ) ) 5 5 5 6 6 6 6 o.6 O.O.4 ,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.Y ) ) 5 5 6 5 5 6 6 6 o.o.o.O.O.+.( , ,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.Y ) 5 5 6 6 6 6 6 o.6 o.o.O.O.+.+.@.[ I ,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.^ 6 5 6 6 6 6 6 o.o.o.O.O.O.+.+.@.@.@.$.! < ,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,./ 6 6 6 6 6 o.o.o.O.O.O.O.@.@.@.@.#.$.$.%.} E ,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,./ o.6 6 o.o.o.o.O.O.O.O.@.@.@.@.#.$.$.%.%.&.&.{ x ,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.4 o.o.o.6 o.O.O.O.+.+.@.@.@.@.$.$.$.%.%.&.&.&.=...V ,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.) o.o.o.O.O.O.O.+.+.@.@.@.$.$.$.} }  . .&.*.=.=.>.;.c ,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,./ o.O.O.O.O.+.+.@.@.[ ' T M m x k k l v b H P -.:.;.c ,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.) +.O.O.+.@.@.] Y 1 - # O O + X 8 8 9 e 7 7 s D F b ,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,./ O.O.+.+.( 3 % . . o O + + X 8 9 0 7 p a g D g @ ,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.( @.+.) > #   . . o O + + 8 8 9 e 7 p f h D y ,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.( ] 2 $   . . o O + + X 8 9 0 r p a g D Z q ,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.m &     . . o O + + 8 8 0 e 7 p f h g C ,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.*   . . o O + + X 8 9 0 r 7 a g D A q ,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.* . o o + + X 8 8 9 e r a f h h i ,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.* o O + + 8 8 9 0 r p a g D A @ ,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.q O + X 8 8 9 e 7 a f h D i ,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.q + 8 8 8 0 r 7 s h h A q ,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.q 8 8 9 e p a f h D y ,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.w 9 0 r r s h h A @ ,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.t r p a f D D z ,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.u 7 s g D Z q ,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.u a g D z ,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.,.i d w ,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.",
",.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,."
};


/* XPM */
static const char *Open_xpm[] = {
"48 48 229 2 ",
"   c #93550D",
".  c #96590E",
"X  c #9C5B0D",
"o  c #935412",
"O  c #9A5D1B",
"+  c #9F6110",
"@  c #A4630E",
"#  c #AB6A0E",
"$  c #AF700F",
"%  c #B2730F",
"&  c #BA7B0F",
"*  c #A66B1D",
"=  c #A86C19",
"-  c #AE7015",
";  c #AD7118",
":  c #B47511",
">  c #BA7C11",
",  c #B77C1D",
"<  c #975E23",
"1  c #915D29",
"2  c #8E5F31",
"3  c #966027",
"4  c #996124",
"5  c #94612E",
"6  c #9A642C",
"7  c #8F6031",
"8  c #916233",
"9  c #986730",
"0  c #986937",
"q  c #976A3A",
"w  c #9B6B38",
"e  c #A06727",
"r  c #A86E22",
"t  c #B27923",
"y  c #B37C2C",
"u  c #A87A36",
"i  c #A57738",
"p  c #B27F36",
"a  c #A17743",
"s  c #BF8214",
"d  c #B88128",
"f  c #B48131",
"g  c #BB8733",
"h  c #BC8835",
"j  c #B78C3F",
"k  c #BC8A3E",
"l  c #C98D0F",
"z  c #C28513",
"x  c #C68B14",
"c  c #C88B12",
"v  c #C18518",
"b  c #C68C1C",
"n  c #CD9314",
"m  c #CB931D",
"M  c #D19613",
"N  c #D49A14",
"B  c #D99F16",
"V  c #D0961F",
"C  c #D49C1A",
"Z  c #DAA216",
"A  c #DAA31B",
"S  c #DDA81F",
"D  c #E2A917",
"F  c #E4AC19",
"G  c #EBB51B",
"H  c #EFB91D",
"J  c #F2BC1E",
"K  c #C78F25",
"L  c #C38C29",
"P  c #CE9927",
"I  c #C6912C",
"U  c #D19A24",
"Y  c #CB9737",
"T  c #CD9B33",
"R  c #C3933B",
"E  c #C7993B",
"W  c #D6A121",
"Q  c #DEAA23",
"!  c #D9A52C",
"~  c #DEA928",
"^  c #DEAF33",
"/  c #E0AB21",
"(  c #E6B323",
")  c #ECB722",
"_  c #E4B32B",
"`  c #E8B72A",
"'  c #E6B82F",
"]  c #EBB92B",
"[  c #F4BF21",
"{  c #E3B636",
"}  c #EBBC34",
"|  c #E0B23C",
" . c #E8BA3A",
".. c #F5C124",
"X. c #F9C427",
"o. c #F6C42C",
"O. c #F9C62C",
"+. c #EEC43F",
"@. c #F5C532",
"#. c #F6C837",
"$. c #F9C935",
"%. c #F2C63C",
"&. c #F7CA3C",
"*. c #F9CB3B",
"=. c #AE854E",
"-. c #BB8D46",
";. c #BF934E",
":. c #AB8552",
">. c #AD8854",
",. c #BB9564",
"<. c #C08F41",
"1. c #C89C46",
"2. c #C09149",
"3. c #CB9F48",
"4. c #C09451",
"5. c #C19B55",
"6. c #C49B5D",
"7. c #CEA24F",
"8. c #D6AA48",
"9. c #DFBD5F",
"0. c #E8BF43",
"q. c #E5BC50",
"w. c #C59E63",
"e. c #C0A167",
"r. c #CCAE64",
"t. c #D5B96F",
"y. c #C7A77D",
"u. c #C8A77B",
"i. c #CBAB7D",
"p. c #D8BB70",
"a. c #D0BA7F",
"s. c #EDC340",
"d. c #ECC54A",
"f. c #F6CC42",
"g. c #F9CE44",
"h. c #F2C949",
"j. c #FAD047",
"k. c #F9D14C",
"l. c #EAC652",
"z. c #EFCB53",
"x. c #EBCB5A",
"c. c #F6D254",
"v. c #F9D454",
"b. c #F4D25C",
"n. c #F9D65B",
"m. c #FBD85B",
"M. c #E3C363",
"N. c #E6C867",
"B. c #EAC961",
"V. c #E3C56C",
"C. c #ECCF6E",
"Z. c #EDD06B",
"A. c #F4D463",
"S. c #F9D760",
"D. c #F9D964",
"F. c #F2D76C",
"G. c #F6DA6C",
"H. c #F9DC6B",
"J. c #EACF70",
"K. c #EFD572",
"L. c #F0D673",
"P. c #F5DC74",
"I. c #F9DE72",
"U. c #F5DE7A",
"Y. c #F9E076",
"T. c #F6E17E",
"R. c #F9E37C",
"E. c #B49D86",
"W. c #BFA991",
"Q. c #CFB699",
"!. c #D0B79A",
"~. c #C2B7AB",
"^. c #D2BEA5",
"/. c #C2BBB4",
"(. c #C5BFB9",
"). c #D9C183",
"_. c #E5CD82",
"`. c #E8D387",
"'. c #EFDB82",
"]. c #EEDD8C",
"[. c #F0DE86",
"{. c #E5D695",
"}. c #E6D69A",
"|. c #E9DB9A",
" X c #F6E282",
".X c #F9E682",
"XX c #F9E887",
"oX c #F7E68B",
"OX c #F8E788",
"+X c #F9E88B",
"@X c #F1E295",
"#X c #F7E996",
"$X c #F8E992",
"%X c #F7E998",
"&X c #D4C1AB",
"*X c #C6C0B8",
"=X c #CBC6BF",
"-X c #D8C7B3",
";X c #D1C5B8",
":X c #DACDBD",
">X c #DFD7B5",
",X c #E5D9A1",
"<X c #E2D7AE",
"1X c #E4D8AB",
"2X c #E2D9BB",
"3X c #CFC9C2",
"4X c #D2CCC5",
"5X c #D3CFCA",
"6X c #DCD1C4",
"7X c #DFDAC2",
"8X c #D5D2CD",
"9X c #DFD5CB",
"0X c #DFDBCD",
"qX c #D7D4D0",
"wX c #D9D7D3",
"eX c #DBD9D6",
"rX c #DEDCDA",
"tX c #E1DBC7",
"yX c #E0D7CC",
"uX c #E0DACD",
"iX c #E2DED3",
"pX c #E1DFDC",
"aX c #E2E0DE",
"sX c #E6E5E4",
"dX c #E8E7E7",
"fX c #E8E8E7",
"gX c #E7E7E8",
"hX c #EAEAEA",
"jX c None",
/* pixels */
"jXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjX",
"jXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjX",
"jXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjX",
"jXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjX",
"jXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjX",
"jXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjX",
"jXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjX",
"jXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjX",
"jXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjX",
"jXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjX=X8X3XjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjX",
"jXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjX(.5XaXhXhXsXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjX",
"jXjXjXjXjXjXjXjXr t jXjXjXjXjXjXjXjXjXjXjXjXjXjX4XpXfXhXhXhXhXfX5XjXjXjXjXjXjXjXjXjXjXjXjXjXjXjX",
"jXjXjXjXjXf 7.M.C.L.N.1.jXjXjXjXjXjXjXjXjX*XwXsXhXhXhXsXsXpXrXrXpXaX4XjXjXjXjXjXjXjXjXjXjXjXjXjX",
"jXjXjXg M.U..XR..X.X.XR.9.y jXjXjXjX/.8XaXhXhXhXsXpXrXrXrXaXsXsXhXhXsX4XjXjXjXjXjXjXjXjXjXjXjXjX",
"jXjXt Z..X.XR..X.X.XR..X.XJ.5.E.3XrXsXhXfXaXrXrXwXrXsXsXhXhXhXhXhXhXhXsXjXjXjXjXjXjXjXjXjXjXjXjX",
"jXjX| R.R..X.X.XR..X.X X|.>XiXsXsXsXrXwXqXwXrXsXfXhXhXhXhXhXhXhXhXhXhXsX*XjXjXjXjXjXjXjXjXjXjXjX",
"jX; %.R.R..X.XR.[.].1X0XsXsXpXrXwXwXeXrXsXfXhXhXhXhXhXhXhXhXhXhXhXhXhXhX8XjXjXjXjXjXjXjXjXjXjXjX",
"jX= @.H..X.X[.,X7XpXpXrXwXqX8XrXsXsXgXhXhXhXhXhXhXhXhXhXhXhXhXhXhXhXhXhXsXjXjXjX= , K P * jXjXjX",
"jXjX~ m.R.].0XsXsXwX8XwXrXsXsXhXhXsXhXhXhXhXhXhXhXhXhXhXhXhXhXhXhXhXuX^.i.k y T E E j u 9 e jXjX",
"jXjXb f.Y. X2XhXsXqXsXhXsXhXsXhXhXhXfXhXhXhXhXhXhXhXhXhXhXhXhXsX^.w.Y !  .z.-.u 9 8 2 2 5 O jXjX",
"jXjX; ] D.R.'.7XsXwXrXhXhXhXhXhXhXhXhXhXhXhXhXhXhXhXhXhXhXhXuXw.U  .S.R.$X{.:.2 2 8 7 8 1 jXjXjX",
"jXjXjXS c.Y.Y.|.sXpXqXgXsXhXsXhXhXhXhXhXhXhXhXhXhXhXhXhXhX-XR ( c.oX%X%X%X@Xe.8 8 2 8 5 O jXjXjX",
"jXjXjXb %.I.I.P.tXgX8XpXhXhXhXhXhXhXhXhXhXhXhXhXhXhXhXhX!.I ` G.#X#X#X#X$X@Xa.0 2 8 8 6 jXjXjXjX",
"jXjXjX% ( D.H.G.}.gXrXwXhXsXhXhXhXhXhXhXhXhXhXhXhXhXiXu.m } P.#X$X#X+X+X$X+X[.a 2 8 2 jXjXjXjXjX",
"jXjXjX+ Z c.H.D.K.iXsXqXsXhXhXhXhXhXhXhXhXhXhXsXyX!.<.C s.T.$X$X$X$XXX+X+XXX[.:.7 8 4 jXjXjXjXjX",
"jXjXjXjXn +.D.D.A.<XhXeXpXhXhXhXhXhXhXsXiX^.u.;.L V _ b.oX+XoX+XXXXXXXXXXXXX Xr.9 1 o jXjXjXjXjX",
"jXjXjXjX> _ S.S.n.`.sXsXrXhXhXhXsX&Xy.4.L b W { z.P..X+XOXOXOX+XXXXX.X.XR.R.R.t.q < jXjXjXjXjXjX",
"jXjXjXjX$ Q c.n.n.A.uXhXqXsXhX6X2.v V ^ l.F..X.X+X+X+X.XXX.X.X.XR.R.R.R.R.R.R.p.w < jXjXjXjXjXjX",
"jXjXjXjX@ N h.n.n.c.1XhXrXsX6Xg N l.U.oXOXOXOX+XXX.X.X.X.XR.R.R.R.R.Y.R.P.I.I.V.p jXjXjXjXjXjXjX",
"jXjXjXjXjXx } v.c.k._.sXaX3X-.n x.+XOX.X+X.X.X.X.X XR.R.R.R.R.Y.Y.Y.I.H.I.G.H.H.R jXjXjXjXjXjXjX",
"jXjXjXjXjX> _ k.k.j.B.iXsX,.l d..X.X.X.XR.R.R.R.R.R.Y.Y.Y.Y.I.H.H.H.H.H.D.H.D.A.f jXjXjXjXjXjXjX",
"jXjXjXjXjX$ Q j.j.j.h.2X^.v { U..XR.T.R.R.R.R.P.I.I.I.Y.H.H.H.H.H.D.D.D.D.D.S.q.=.jXjXjXjXjXjXjX",
"jXjXjXjXjX# C &.f.g.g.).k C A.R.R.R.P.Y.I.I.I.I.I.H.H.H.H.D.D.D.D.D.S.S.m.n.m.8.W.jXjXjXjXjXjXjX",
"jXjXjXjXjXX n %.g.*.*.I c 0.I.Y.I.I.I.I.H.I.H.H.H.D.H.D.D.D.D.S.S.n.n.n.n.v.v.7.;XjXjXjXjXjXjXjX",
"jXjXjXjXjXjXx ] *.*.] > Q G.G.I.G.H.H.H.H.H.D.D.D.S.S.S.S.m.n.n.v.n.n.v.v.v.h.4.~.jXjXjXjXjXjXjX",
"jXjXjXjXjXjX> ( $.$.U l d.H.H.G.H.H.D.H.D.D.D.S.D.m.m.m.n.c.n.v.v.v.v.j.g.#.` * jXjXjXjXjXjXjXjX",
"jXjXjXjXjXjX% D $.] s A c.H.S.S.D.D.S.S.S.n.n.m.n.v.v.v.v.v.v.k.f.&.@.[ [ [ A jXjXjXjXjXjXjXjXjX",
"jXjXjXjXjXjX@ Z X.C x ] v.S.S.S.S.m.n.n.n.n.v.c.v.v.v.v.j.f.#.o.[ [ [ [ [ [ v jXjXjXjXjXjXjXjXjX",
"jXjXjXjXjXjX@ N ) v A @.g.n.m.n.m.v.v.v.v.c.v.v.k.k.f.@.[ X.[ [ [ [ [ [ J n . jXjXjXjXjXjXjXjXjX",
"jXjXjXjXjXjXjXn B s ( O.#.v.v.v.v.v.v.v.k.j.&.#.[ [ [ [ [ [ [ [ [ G D M > X jXjXjXjXjXjXjXjXjXjX",
"jXjXjXjXjXjXjXc s M J X.o.k.v.k.k.k.g.#.@.[ ..[ J ..[ [ J G D M z $ @ jXjXjXjXjXjXjXjXjXjXjXjXjX",
"jXjXjXjXjXjXjX& > A X.X...f.f.#.o.X.[ [ [ [ [ [ J G D M z $ @ jXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjX",
"jXjXjXjXjXjXjX# z G ........X...[ ..[ [ [ H D N c % @   jXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjX",
"jXjXjXjXjXjXjX+ M H ......[ ..J J H F N c : @   jXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjX",
"jXjXjXjXjXjXjXX N J [ [ J H F Z c & @ X jXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjX",
"jXjXjXjXjXjXjXjX% D F Z c & #   jXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjX",
"jXjXjXjXjXjXjXjXjX# # X jXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjX",
"jXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjXjX"
};


static const char *NfsOpen_xpm[] = {
/* columns rows colors chars-per-pixel */
"48 48 235 2 ",
"   c #5E3817",
".  c #583618",
"X  c #5F3A19",
"o  c #653917",
"O  c #6B3A17",
"+  c #663D1A",
"@  c #6C3D1A",
"#  c #773D17",
"$  c #723D18",
"%  c #7A3E19",
"&  c #72401C",
"*  c #7B401A",
"=  c #664423",
"-  c #694524",
";  c #6D4827",
":  c #6E4928",
">  c #734927",
",  c #724B2A",
"<  c #7B4E2B",
"1  c #7A5231",
"2  c #853F19",
"3  c #84421B",
"4  c #8C441B",
"5  c #94451B",
"6  c #9D471C",
"7  c #9E491E",
"8  c #A0471B",
"9  c #A44B1E",
"0  c #AB4C1D",
"q  c #B34F1E",
"w  c #BF501E",
"e  c #864722",
"r  c #814A24",
"t  c #884923",
"y  c #924B24",
"u  c #9E4D23",
"i  c #91502B",
"p  c #9C532C",
"a  c #895733",
"s  c #815937",
"d  c #845F3E",
"f  c #965632",
"g  c #935D3C",
"h  c #A44F23",
"j  c #A94E21",
"k  c #B14F21",
"l  c #A35429",
"z  c #A95328",
"x  c #A95A2F",
"c  c #B45223",
"v  c #BB5424",
"b  c #B25628",
"n  c #B85728",
"m  c #B65B2E",
"M  c #BE5C2C",
"N  c #A35830",
"B  c #AB5F36",
"V  c #B15D31",
"C  c #8F633F",
"Z  c #91613B",
"A  c #9B633E",
"S  c #BF6132",
"D  c #C3521F",
"F  c #CB541F",
"G  c #CB5520",
"H  c #C15825",
"J  c #C65E2E",
"K  c #C85B2B",
"L  c #D35720",
"P  c #D55A23",
"I  c #DB5921",
"U  c #D35F2B",
"Y  c #E65C22",
"T  c #EA5D22",
"R  c #F15F22",
"E  c #CC612F",
"W  c #D7612D",
"Q  c #DC642D",
"!  c #CD6636",
"~  c #C8673A",
"^  c #C96B3B",
"/  c #D06A3A",
"(  c #DA6D3C",
")  c #DB703D",
"_  c #E76229",
"`  c #EF652B",
"'  c #F36023",
"]  c #F3662B",
"[  c #F4682E",
"{  c #E16833",
"}  c #F46C33",
"|  c #EB713C",
" . c #F6723B",
".. c #8E6548",
"X. c #8C6B4F",
"o. c #906646",
"O. c #9B6647",
"+. c #936A4C",
"@. c #9C6C4C",
"#. c #906F51",
"$. c #9A7055",
"%. c #AC764E",
"&. c #A57B5C",
"*. c #A97B5F",
"=. c #B17951",
"-. c #9A7D67",
";. c #A57E64",
":. c #CC7247",
">. c #D67949",
",. c #DE7A49",
"<. c #ED7440",
"1. c #EC7947",
"2. c #E17E4D",
"3. c #E87D4B",
"4. c #F57641",
"5. c #F97741",
"6. c #F67A45",
"7. c #F87B46",
"8. c #F67E49",
"9. c #F87D48",
"0. c #B2805C",
"q. c #AE8665",
"w. c #AA8F7B",
"e. c #AC917D",
"r. c #B0917D",
"t. c #BC977A",
"y. c #C2865D",
"u. c #EE804E",
"i. c #F6814D",
"p. c #F8824D",
"a. c #E48351",
"s. c #EF8651",
"d. c #E88957",
"f. c #E2855B",
"g. c #E8895C",
"h. c #F38552",
"j. c #F88551",
"k. c #F58A56",
"l. c #F88955",
"z. c #F78E5B",
"x. c #F88D59",
"c. c #EC905F",
"v. c #F5915E",
"b. c #F8925E",
"n. c #CD9166",
"m. c #D19064",
"M. c #D6976B",
"N. c #DC996D",
"B. c #CB977D",
"V. c #C29C7C",
"C. c #DC9A7A",
"Z. c #E79060",
"A. c #EA9564",
"S. c #E7976C",
"D. c #E29A6C",
"F. c #EE9A6A",
"G. c #F69563",
"H. c #F89460",
"J. c #F69967",
"K. c #F99A66",
"L. c #F69E6C",
"P. c #F99D69",
"I. c #E29D70",
"U. c #F7A06F",
"Y. c #F9A16F",
"T. c #E3A072",
"R. c #ECA576",
"E. c #ECA579",
"W. c #ECA97D",
"Q. c #F6A373",
"!. c #F9A573",
"~. c #F7A877",
"^. c #F9A877",
"/. c #F6AA7A",
"(. c #F9AC7C",
"). c #FBB07F",
"_. c #AC9381",
"`. c #A79788",
"'. c #A7988B",
"]. c #B7A597",
"[. c #B8A799",
"{. c #B7AEA6",
"}. c #BCB4AD",
"|. c #BEB8B3",
" X c #D0A382",
".X c #D4A887",
"XX c #DDAD90",
"oX c #DDB294",
"OX c #E3A985",
"+X c #EAA981",
"@X c #E0AE8E",
"#X c #F8AF80",
"$X c #EBB88F",
"%X c #F1B184",
"&X c #F9B283",
"*X c #F9B788",
"=X c #F7BB8E",
"-X c #F9BA8C",
";X c #E1AE91",
":X c #EFBC91",
">X c #ECBF9A",
",X c #F4BD92",
"<X c #F9BE91",
"1X c #C0B2A6",
"2X c #C1B4A9",
"3X c #DBB8A4",
"4X c #C4BAB2",
"5X c #C8BDB4",
"6X c #C6BEB8",
"7X c #EFC19B",
"8X c #F9C295",
"9X c #F9C59A",
"0X c #CBC3BE",
"qX c #DCC2B2",
"wX c #DBC5B9",
"eX c #CCC6C2",
"rX c #CEC9C5",
"tX c #D3CBC5",
"yX c #DDCEC5",
"uX c #D1CDCA",
"iX c #D5D0CB",
"pX c #DBD2CB",
"aX c #D6D4D2",
"sX c #D9D6D4",
"dX c #DBD9D7",
"fX c #DDDCDA",
"gX c #E1DAD6",
"hX c #E0DFDE",
"jX c #E1E0DF",
"kX c #E5E4E4",
"lX c #E8E7E7",
"zX c #E8E8E7",
"xX c #E7E7E8",
"cX c #EAEAEA",
"vX c None",
/* pixels */
"vXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvX",
"vXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvX",
"vXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvX",
"vXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvX",
"vXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvX",
"vXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvX",
"vXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvX",
"vXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvX",
"vXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvX",
"vXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvX}.eX0XvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvX",
"vXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvX|.uXfXzXcXhXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvX",
"vXvXvXvXvXvXvXvX< a < vXvXvXvXvXvXvXvXvXvXvXvXvXtXfXlXcXcXcXzXlXrXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvX",
"vXvXvXvXvXZ =.m.T.R.M.%.> vXvXvXvXvXvXvXvX{.uXkXcXcXcXzXkXfXfXfXjXjXeXvXvXvXvXvXvXvXvXvXvXvXvXvX",
"vXvXvXA M.W.&X&X#X#X).).m.Z vXvXvXvX}.rXaXkXcXcXlXhXfXsXfXhXlXzXcXcXkX0XvXvXvXvXvXvXvXvXvXvXvXvX",
"vXvXr D.&X).(.).).(.(.).(.I.&.`.rXdXlXcXcXkXfXsXsXfXkXlXcXcXcXcXcXcXcXfXvXvXvXvXvXvXvXvXvXvXvXvX",
"vXvXV (.).(.(.(.(.(.^.W.@XqXpXkXlXlXfXaXaXsXfXlXcXcXcXcXcXzXcXcXcXcXcXxX6XvXvXvXvXvXvXvXvXvXvXvX",
"vX# J Y.(.(.(.(.E.OX3XyXjXkXjXdXaXaXsXhXkXcXcXcXcXcXcXcXcXcXcXcXcXcXcXcXaXvXvXvXvXvXvXvXvXvXvXvX",
"vXO k A.(.~.W.oXwXfXjXfXsXaXsXdXkXlXcXcXcXcXcXcXcXcXcXcXcXcXcXcXzXcXcXcXkXvXvXvX@ 4 9 j * vXvXvX",
"vXvX8 >.^.+XpXkXxXsXaXsXhXkXkXcXxXcXcXcXcXcXcXcXcXcXcXcXcXcXcXcXcXzXpX1Xr.Z t i y p p r > 1 vXvX",
"vXvX4 J L.~.qXzXkXaXjXcXcXcXcXzXxXxXcXcXcXcXcXcXcXcXcXcXcXcXcXkX1X&.B M Q :.Z < ; - - - , > vXvX",
"vXvX# k c.Y.E.pXxXaXfXcXcXzXzXzXcXcXcXcXcXcXcXcXcXcXcXcXcXcXpX;.c Q s.^.*XoX#.= = = = ; 1 vXvXvX",
"vXvXvX8 ,.Y.P.;XkXjXaXxXzXcXcXcXcXcXcXcXcXcXcXcXcXcXcXcXcX5XA G s.&X9X9X9X7Xq.- = = ; , : vXvXvX",
"vXvXvX4 ! P.L.F.yXcXaXjXcXkXcXcXcXcXcXcXcXcXcXcXcXcXcXzX[.N P A.8X9X9X9X9X7XV., = = : 1 vXvXvXvX",
"vXvXvX% v H.L.K.XXkXfXsXcXcXcXcXcXcXcXcXcXcXcXcXcXcXfXw.h W Q.8X8X8X8X8X8X9X$Xd = = 1 1 vXvXvXvX",
"vXvXvXO 0 a.P.K.S.sXcXaXcXcXcXcXcXcXcXcXcXcXcXjXiX[.O.q ) /.<X<X8X8X<X8X8X8X$X+.= ; 1 vXvXvXvXvX",
"vXvXvXvX5 ) K.K.v.3XcXdXhXcXcXcXcXcXcXkXsX2X_.@.p j E d.#X-X=X-X=X=X=X8X8X<X<Xq.= - + vXvXvXvXvX",
"vXvXvXvX4 J b.b.z.C.jXkXsXcXcXcXhX4X_.$.f h b ~ a.F.(.*X-X-X-X-X-X-X-X-X-X<X<Xt., , vXvXvXvXvXvX",
"vXvXvXvX% c k.x.z.g.yXcXsXkXcXeX+.5 h S >.Z.Q.(.#X&X&X&X#X&X&X&X-X&X-X-X-X-X<X X1 1 vXvXvXvXvXvX",
"vXvXvXvXO 9 3.x.x.i.3XcXdXkXtXg h >.J.Q.~.(.(.(.)./.(.).#X).&X&X&X%X*X-X*X-X-X.X+.vXvXvXvXvXvXvX",
"vXvXvXvXvX5 ) j.j.8.C.xXkXeX+.6 >.Y.Y.!.~.~.Y.~.~.(./.(.).).(.#X).&X&X&X&X&X*X%X@.vXvXvXvXvXvXvX",
"vXvXvXvXvX2 J 8.8.p.f.gXkX-.4 ^ K.P.L.U.L.Y.Q.Q.Q.~.~.~.~./.(.(./.).).).&X&X&XW.C vXvXvXvXvXvXvX",
"vXvXvXvXvX% v 7.7.7.1.wX}.e m G.K.K.J.L.L.L.L.L.Q.Q.Y.Q.~.~.~./.^.(./.(.(.(.#XN.X.vXvXvXvXvXvXvX",
"vXvXvXvXvXO k <.6.6.4.B...u ,.H.H.H.J.J.J.K.K.K.L.L.Q.U.Q.Q.Q.~.Q.~.~.~.(.(.(.y.`.vXvXvXvXvXvXvX",
"vXvXvXvXvXO 6 { 5.4. .x 4 ~ h.z.v.v.b.H.H.J.J.J.K.L.L.P.L.L.U.Y.Q.Q.~.Y.!.^.!.0.0XvXvXvXvXvXvXvX",
"vXvXvXvXvXvX2 W  . .{ y z 3.h.k.x.x.x.b.v.H.H.J.G.G.J.K.K.P.P.P.L.U.L.Q.!.Y.F.;.{.vXvXvXvXvXvXvX",
"vXvXvXvXvXvX2 K } } n 4 / j.8.j.l.l.l.l.x.x.z.z.b.H.H.H.H.J.K.P.P.K.K.J.H.l.,.< vXvXvXvXvXvXvXvX",
"vXvXvXvXvXvX% c } Q 4 9 <.j.8.8.j.j.l.l.l.l.l.l.x.z.z.b.b.H.H.b.z.j.p.6. .7.! vXvXvXvXvXvXvXvXvX",
"vXvXvXvXvXvX$ 0 ` v 2 F } 8.8.8.i.i.j.j.j.l.l.l.j.x.x.l.l.p.7. . .} }  . . .l vXvXvXvXvXvXvXvXvX",
"vXvXvXvXvXvXO h I 5 8 Y [ 6.6.4.i.i.i.i.j.j.j.j.i.9.4. .} [ [ [ } [ } } [ S @ vXvXvXvXvXvXvXvXvX",
"vXvXvXvXvXvXvX5 k 4 w R ` 4.6.4.7.7.7.i.7.4. .} [ ] ' R ' ' ] ] ] _ U n y + vXvXvXvXvXvXvXvXvXvX",
"vXvXvXvXvXvXvX5 4 5 I R R  .5.4.4.4. .} [ ] ' R ' ' ' ' ' Y I v u 3 & vXvXvXvXvXvXvXvXvXvXvXvXvX",
"vXvXvXvXvXvXvX* 2 j R R R  .} ] [ ' ' ' ' ' R R R Y P v 9 3 @ X vXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvX",
"vXvXvXvXvXvXvX@ 3 G R R R ' R R ' R ' ' R Y I D 9 4 $ . vXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvX",
"vXvXvXvXvXvXvXo 4 I R R ' ' ' R ' Y I w 9 4 #   vXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvX",
"vXvXvXvXvXvXvX+ 8 Y R ' R Y I F 0 4 * o vXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvX",
"vXvXvXvXvXvXvXvX3 L I G k 6 % o vXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvX",
"vXvXvXvXvXvXvXvXvX% % o vXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvX",
"vXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvXvX"
};

static const char *LogScale_xpm[] =
{
"34 34 168 2 ",
"   c #010101",
".  c #080806",
"X  c #0B0B0B",
"o  c #100E0F",
"O  c #13140F",
"+  c #141413",
"@  c #181713",
"#  c #1D1C1C",
"$  c #242323",
"%  c #2B2B2B",
"&  c #262E35",
"*  c #2A3139",
"=  c #373735",
"-  c #383836",
";  c #393738",
":  c #3C3C39",
">  c #43433F",
",  c #192E46",
"<  c #122F4F",
"1  c #1A3047",
"2  c #1B3148",
"3  c #112E50",
"4  c #143251",
"5  c #1F3751",
"6  c #213952",
"7  c #374047",
"8  c #2B425A",
"9  c #28435B",
"0  c #33485C",
"q  c #2E4660",
"w  c #274663",
"e  c #324A63",
"r  c #3E5364",
"t  c #3B5671",
"y  c #444442",
"u  c #4B4C4B",
"i  c #51514E",
"p  c #4C535B",
"a  c #525251",
"s  c #5D5D5D",
"d  c #5B5C56",
"f  c #62635D",
"g  c #44596E",
"h  c #425A72",
"j  c #4A627A",
"k  c #646464",
"l  c #666866",
"z  c #6B6A65",
"x  c #666769",
"c  c #6C6B6A",
"v  c #6F706B",
"b  c #72736E",
"n  c #6F7170",
"m  c #737373",
"M  c #7A7B7A",
"N  c #7A7B76",
"B  c #828278",
"V  c #566E86",
"C  c #567087",
"Z  c #5C778D",
"A  c #5F788E",
"S  c #5D7283",
"D  c #5C7B97",
"F  c #617585",
"G  c #64798C",
"H  c #6C7E8A",
"J  c #717E87",
"K  c #717E88",
"L  c #657B90",
"P  c #75838C",
"I  c #7B848B",
"U  c #65829D",
"Y  c #6C8299",
"T  c #74899A",
"R  c #6C89A7",
"E  c #6E8BA9",
"W  c #6584A1",
"Q  c #748CA5",
"!  c #788EA2",
"~  c #728CAC",
"^  c #7693B3",
"/  c gray52",
"(  c #868887",
")  c #8C8D8B",
"_  c #8F9088",
"`  c #929292",
"'  c #999996",
"]  c #9D9D9A",
"[  c #879AAB",
"{  c #859EB2",
"}  c #92A1AD",
"|  c #98A1A8",
" . c #90A3B4",
".. c #AAABAA",
"X. c #A5A5A5",
"o. c #B1B1AF",
"O. c #A5ACB6",
"+. c #A0AEB9",
"@. c #A3B3BD",
"#. c #B5B4B1",
"$. c #B9B8B5",
"%. c #B5BDBE",
"&. c #BDBCBA",
"*. c #C0BFBB",
"=. c #C2C2BD",
"-. c #9AACC2",
";. c #96B2CE",
":. c #A4B3C3",
">. c #AAB7C0",
",. c #AEBCC7",
"<. c #AFBDC8",
"1. c #B7BEC8",
"2. c #B5BCC4",
"3. c #A0B8D0",
"4. c #AEBED5",
"5. c #B7C3CE",
"6. c #B9C4CC",
"7. c #B9C1C3",
"8. c #ABC3DB",
"9. c #BAC7D7",
"0. c #B5C8D6",
"q. c #BECAD3",
"w. c #ADC7E2",
"e. c #B3CBE4",
"r. c #B8D0EA",
"t. c #C2C2C1",
"y. c #CBCBCB",
"u. c #D5D6CF",
"i. c #C8CED3",
"p. c #C7D1D3",
"a. c #CDD5D5",
"s. c #CFD8D7",
"d. c #C8D3D9",
"f. c #D2D6D5",
"g. c #D4D8D5",
"h. c #DBDDD5",
"j. c #D3D9D9",
"k. c #DDDDDB",
"l. c #DFE0DB",
"z. c #E3E2DA",
"x. c #EBEADE",
"c. c #D1DBE2",
"v. c #DBE2E7",
"b. c #D4E1F2",
"n. c #E4E5E4",
"m. c #EDEBE4",
"M. c #E4E7ED",
"N. c #E3EBEE",
"B. c #EAEBEB",
"V. c #F1EDE5",
"C. c #F2EEEA",
"Z. c #EEF0EF",
"A. c #F6F5EE",
"S. c #F8F5EE",
"D. c #E4EEF0",
"F. c #E9EDF5",
"G. c #E2EEFE",
"H. c #E9EDF8",
"J. c #E6F2FF",
"K. c #ECF4FB",
"L. c #EAFFFF",
"P. c #F6F6F3",
"I. c #F8F7F5",
"U. c #F7F8F5",
"Y. c #FAFAF4",
"T. c #F5F7F8",
"R. c #F6FEFF",
"E. c #FCFEFE",
/* pixels */
"E.E.E.E.E.E.E.E.E.E.E.E.E.E.E.E.E.E.E.E.E.E.E.E.E.E.E.E.E.E.E.E.E.E.",
"E.E.R.E.E.E.E.E.E.R.E.E.E.E.R.E.E.E.E.E.R.E.E.E.E.E.R.R.E.E.E.E.E.E.",
"E.i. .Z t e e e e e e e q 9 6 6 5 6 5 5 6 8 8 e e e e e t G >.v.E.E.",
"E.} Y ~ ;.e.r.r.e.e.r.r.8.{ C h h h h h j Q 3.e.e.e.e.w.;.U ! +.E.E.",
"R.F Q 4.J.E.O.1.R.R.O.2.R.| p * & & & * 7 I c.E.R.E.E.E.b.-.W C R.E.",
"L.g :.G.K.t.% x T.y.s c $.c # o = = ; $ . i t.E.E.E.E.E.E.b.~ w E.E.",
"L.e 9.R.p.% X M f.% ..' # a # a n.B.n.` X a t.E.E.E.E.E.E.R.^ 4 R.E.",
"L.0 q.E.A.o.% m y.# #.] o a $ # s B.` % X a t.E.E.E.E.E.T.K.~ 4 E.E.",
"L.0 q.E.E.Y.> b y.@ #.] . a $ $ / B...u X i *.E.E.Y.E.Y.S.K.~ < R.E.",
"L.0 6.E.Y.P.: c u.y ' _ = s # u g.I.n./ X i *.E.Y.Y.I.I.I.K.~ < E.E.",
"L.0 5.E.T.C.> x I.k.a c y.m # . ; k.N o X a *.T.Y.I.I.T.P.K.~ 3 R.E.",
"L.e 5.E.T.P.*.y.T.I.*.y.n.m #   @ k.s   X a t.T.I.P.T.Y.P.K.~ 3 E.E.",
"L.e 5.R.T.T.E.E.I.I.E.E.l.m # % / F...u X i 2.T.I.I.T.I.A.K.~ < R.E.",
"L.0 5.E.T.T.T.P.I.I.T.T.n.m + i k.T.B./ o a *.I.T.T.I.S.m.F.~ 3 E.E.",
"L.0 5.R.T.P.P.T.T.T.I.T.n.n # X : k.M o X a 2.I.I.P.I.C.m.H.~ 3 E.E.",
"L.0 5.E.T.P.P.P.I.T.T.Y.k.n #   # k.k   X a *.Y.P.I.I.C.m.H.~ 3 E.E.",
"L.0 6.R.U.P.Y.P.U.P.U.Y.l.n $   # k.k   . a &.E.P.A.S.A.m.F.Q 3 R.E.",
"L.0 6.R.U.A.A.P.A.A.P.Y.l.v # X y n.M + X a &.Y.P.A.A.S.V.M.E 3 E.E.",
"L.0 6.R.P.A.A.A.A.A.A.Y.l.v @ a k.E.B.) X i *.Y.A.A.S.A.V.M.E < E.E.",
"L.0 6.R.P.A.A.A.S.A.A.Y.l.b # $ M M.X.y X i &.Y.A.A.P.S.V.M.Q < E.E.",
"L.0 6.R.A.A.A.A.A.A.Y.Y.l.b #   # k.k   X i &.Y.A.A.A.A.V.B.Q 3 R.E.",
"L.0 5.Y.P.A.A.A.Y.A.z.m.l.b #   $ k.x   X i &.Y.A.A.A.A.V.M.E 3 E.E.",
"L.0 6.R.A.S.A.A.A.A.f N l.b $   $ k.k   X i &.Y.A.A.S.A.m.D.E 3 R.E.",
"L.0 6.R.A.A.A.Y.h.z @ f l.b #   $ k.x   X i &.Y.A.A.P.A.m.D.E 3 E.E.",
"L.0 6.R.A.A.A.Y.u.i O z l.b #   # k.k   . i &.Y.Y.A.A.A.m.D.E < R.E.",
"L.0 6.R.A.A.A.S.A.m.> f l.b # . ; z.m O X i &.Y.Y.A.A.m.m.N.E < E.E.",
"L.0 1.E.A.A.Y.A.Y.C.> f l.b @ u j.E.n.( X i &.Y.A.A.C.m.A.D.E < R.E.",
"L.0 1.U.A.C.A.A.A.m.> d l.v + % m n b u . i $.Y.A.A.C.m.m.v.R < E.E.",
"L.0 >.D.m.x.x.x.m.z.z B h._ : @ + + + o $ b =.x.x.x.m.z.l.c.W 4 R.E.",
"L.r } d.j.l.h.h.h.h.u.u.h.g.) f x l x k b =.l.l.g.l.g.u.a.,.D 9 E.E.",
"R.S G [ 7.a.a.s.a.a.f.s.g.f.6.%.%.%.%.%.%.p.s.p.g.s.a.p.5.T Z Z E.E.",
"R.@.! G G H J P J J J J J P P P P P P P P P P M P P H P Z Z { 0.E.E.",
"E.v.@.S 0 , , , , , , , , , , , , , , , , , , , , , , 1 8 S 5.R.E.E.",
"E.E.R.L.L.L.L.L.L.L.L.L.L.L.L.L.L.L.L.L.L.L.L.L.L.L.L.L.L.L.E.E.E.E."
};

#endif

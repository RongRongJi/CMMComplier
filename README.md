# CMMComplier
CMM语言解释器
## 词法分析
### 词法分析主要功能
a)忽略空格、tab键、回车换行等分隔符；<br>
b)识别不同类型的记号；<br>
c)识别并忽略注释；<br>
d)记录下每个记号的行号与列号；<br>
e)将识别的记号输出；<br>
f)如果输入串存在词法分析错误，则报错。<br>
### 种别码表
|符号         | 编码       |符号  |编码 | 符号 | 编码 |
| ------------- |:-------------:| ------------- |:-------------:| ------------- |:-------------:|
| NULL | 0 | ERROR | 1 | IF | 2 |
| ELSE | 3 | WHILE | 4 | FOR | 5 |
| READ | 6 | WRITE | 7 | INT | 8|
| REAL | 9 | CHAR | 10 | VOID | 11 |
|+|12|-|13|* |14|
|/ |15|=|16|<|17|
|>|18|==|19|<>|20|
|(|21|)|22|;|23|
|{|24|}|25|/* |26|
|* /|27|// |28|\[ 29|
|] |30| '|31|"|32|
|<=|33|>=|34|id|35|
|return|36|int_value|37|real_value|38|
|char_value|39|str_value|40|||

## 语法分析
### 文法
program -> stmt-sequence <br>
stmt-sequence -> statement ; stmt-sequence | statement | ε <br>
statement -> if-stmt | while-stmt |  read-stmt | write-stmt | for-stmt |declare-stmt | funcall-stmt | assign-stmt | return<br>
stmt-block -> statement | { stmt-sequence } <br>
if-stmt -> if ( exp ) then stmt-block | if ( exp ) then stmt-block else stmt-block <br>
while-stmt -> while ( exp ) stmt-block <br>
for-stmt -> for ( assign-stmt ; exp ; assign-stmt ) stmt-block<br>
assign-stmt -> variable = exp ; <br>
read-stmt -> read ( variable ); <br>
write-stmt -> write ( exp ); <br>
declare-stmt -> (int | real) ( (identifier [= exp ]) | (identifier [ exp ]) ) ; | (int | real ) functionDeclare-stmt stmt-block<br>
functionDeclare-stmt ->  identifier ( declare-stmt ) <br>
funcall-stmt -> identifier ( exp ) ;<br>
variable -> identifier [ [ exp ] ] <br>
exp -> addtive-exp logical-op addtive-exp | addtive-exp<br>
addtive-exp -> term add-op additive-exp | term <br>
term -> factor mul-op term | factor <br>
factor -> ( exp ) | number | variable | Add-op exp <br>
logical-op -> > | < | >= | <= | <> | == <br>
add-op -> + | - <br>
mul-op -> * | / <br>
return -> return exp ;<br>

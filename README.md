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
|* /|27|// |28|\[ | 29|
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

## 语义分析
### 中间代码
中间代码用四元式表示，具体表示方式如下：<br>
#### if语句四元式
(JMP, 判断表达式, , )
(IN, , , )
(函数主体操作)
(OUT, , ,)
如果有else则进行下面操作
(JMP, , ,) 
(IN, , ,)
(OUT, , ,)
#### while语句四元式
(JMP,判断表达式, ,)
(IN, , ,)
(函数主体操作)
(OUT, , ,)
(JMP, , ,返回函数主体)
#### for语句四元式
(JMP,判断表达式, , )
(IN, , ,)
(函数主体操作)
(OUT, , ,)
(JMP,函数主体行数, , )
#### read语句四元式
(READ, , ,存储变量)
#### write语句四元式
(WRITE, , ,输出变量)
#### 声明变量语句四元式
(INT/REAL/CHAR, 值, , 变量)
#### 赋值语句四元式
(ASSIGN,值, ,变量)
#### 函数声明四元式
(INT/REAL/VOID/CHAR, , ,函数名())
(IN, , ,)
(函数主体操作)
(OUT, , ,)
(JMP, , ,返回调用函数前的语句序号)
#### return语句四元式
(ASSIGN,返回值, ,函数返回所赋的变量)
(JMP, ,返回调用函数时的四元式序号)
#### 函数调用四元式
(ASSIGN,0, ,栈顶)
(ASSIGN,依次将参数赋值到栈顶)
(ASSIGN,函数起始地址 , ,栈顶)
(CALL, , ,函数名)
(JMP, , ,函数所在四元式序号)
(CALLFH, , ,)
#### 逻辑表达式四元式
(符号,左值,右值,临时变量)
#### 多项式四元式
(加减符号,左值,右值,临时变量)
#### 项四元式
(乘除符号,左值,右值,临时变量)
### 四元式解释执行
四元式执行：<br>
JMP:<br>
第二元进行判断，如果非0则执行JMP，跳转到第四元所指向的四元式<br>
READ:<br>
根据第四元的变量类型，声明该类型的变量，并调用cin输入值<br>
WRITE:<br>
根据第四元的变量的值存入returnValue，调用cout输出returnValue<br>
IN:<br>
将旧栈顶存入栈集中，增加层数<br>
OUT:<br>
减少层数，撤销该层的所有符号表，将旧栈顶弹出<br>
INT/REAL/CHAR/VOID:<br>
通过符号表查询所需的变量长度，申请空间<br>
ASSIGN:<br>
将第二元的值赋值给第四元的变量<br>
PLUS/MINUS/MUL/DIV/GT/GET/LT/LET/EQ/NEQ:<br>
将第二元第三元的值进行第一元所指的运算操作，并把结果保存在第四元的临时变量中<br>
CALL:<br>
将旧栈底存入栈集中，新的栈底为栈顶-1<br>
CALLFH:<br>
新栈顶为原栈底，新栈底从栈集中弹出<br>

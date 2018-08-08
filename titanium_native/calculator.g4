grammar calculator;

/*
 * Parser rules
 */

texpression
   : (tterm)+
   ;

tterm
   : tliteral
   | toperator
   | tprocedure
   ;

tprocedure
   : '(' texpression ')'
   ;

tliteral
   : NUMBER
   ;

toperator
   : PLUS
   | MINUS
   | TIMES
   | DIV
   ;

/*
 * Lexer rules
 */

PLUS
   : '+'
   ;


MINUS
   : '-'
   ;


TIMES
   : '*'
   ;


DIV
   : '/'
   ;

NUMBER
   : ('0' .. '9') + ('.' ('0' .. '9') +)?
   ;

WS
   : [ \r\n\t] + -> skip
   ;

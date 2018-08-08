grammar calculator;

/*
 * Parser rules
 */

tnExpression
   : (tnTerm)+
   ;

tnTerm
   : tnLiteral
   | tnOperator
   | tnProcedure
   ;

tnProcedure
   : '(' tnExpression ')'
   ;

tnLiteral
   : NUMBER
   ;

tnOperator
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

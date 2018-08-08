grammar titaniumNative;

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
   : TN_NUMBER
   ;

tnOperator
   : TN_PLUS
   | TN_MINUS
   | TN_TIMES
   | TN_DIV
   ;

/*
 * Lexer rules
 */

TN_PLUS
   : '+'
   ;


TN_MINUS
   : '-'
   ;


TN_TIMES
   : '*'
   ;


TN_DIV
   : '/'
   ;

TN_NUMBER
   : ('0' .. '9') + ('.' ('0' .. '9') +)?
   ;

TN_WS
   : [ \r\n\t] + -> skip
   ;

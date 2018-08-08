grammar TitaniumNative;

/*
 * Parser rules
 */

tnExpression
   : (tnTerm)+ EOF
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
   | TN_BOOL
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
   : ('0' .. '9')+ ('.' ('0' .. '9')+)?
   ;

TN_BOOL
   : 'true'
   | 'false'
   ;

TN_WS
   : [ \r\n\t]+ -> skip
   ;

TN_INVALID
   : .+?
   ;

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
   : tnWordOperator
   | tnVariableOperator
   | tnSymbolOperator
   ;

tnWordOperator
   : TN_IDENTIFIER
   ;

tnVariableOperator
   : '=' TN_IDENTIFIER
   | '!' TN_IDENTIFIER
   ;

tnSymbolOperator
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

TN_IDENTIFIER
   : TN_LETTER TN_LETTER_OR_DIGIT*
   ;

fragment TN_LETTER
   : [a-zA-Z_]
   ;

fragment TN_LETTER_OR_DIGIT
   : TN_LETTER
   | [0-9]
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

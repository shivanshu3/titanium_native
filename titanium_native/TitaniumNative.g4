grammar TitaniumNative;

/*
 * Parser rules
 */

// This is the root node
tnProgram
   : tnExpression EOF
   ;

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
   | TN_BOOL
   | tnString
   ;

tnString
   : TN_NON_VERBATIM_STRING
   | TN_VERBATIM_STRING
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
   : '++'
   | '--'
   | '[]'
   | '<='
   | '>='
   | '=='
   | '+'
   | '-'
   | '*'
   | '/'
   | '<'
   | '>'
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

TN_NON_VERBATIM_STRING
   : '`' TN_NON_VERBATIM_CHAR* '`'
   ;

TN_VERBATIM_STRING
   : '@' '`' TN_VERBATIM_CHAR* '`'
   ;

fragment TN_NON_VERBATIM_CHAR
   : ~["\\\r\n]
   | TN_ESCAPE_SEQUENCE
   ;

fragment TN_VERBATIM_CHAR
   : ~[\u0000-\u001F\u0060]
   ;

fragment TN_ESCAPE_SEQUENCE
   : '\\' [btnfr"'\\]
   ;

TN_WS
   : [ \r\n\t]+ -> skip
   ;

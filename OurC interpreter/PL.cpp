# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <cstdio> // sprintf
# include <map>
# include <stack>
# include <sstream> //
# include <vector>
# include <iostream> // strtof
# include <cmath> // floor
# include <string>
# define NOT !
# define AND &&
# define OR ||
# define EQ ==
using namespace std;
enum TokenType
{ IDENTIFIER = 17222, NUM = 12345, SIGN = 23456, QUIT = 33333, OTHERS = 11111, UNRECOGNIZE = 22212 } ;
typedef char * CharPtr ;

struct Define {
  CharPtr name;
  CharPtr num;
  Define * next;
};

typedef Define * DefinePtr;

struct Column {
  int column ;
  Column * next ;
} ; // Column

typedef Column * ColumnPtr ;

struct Line {
  int line ;
  ColumnPtr firstAppearAt ;
  ColumnPtr lastAppearAt ;
  Line * next ;
} ; // Line

typedef Line * LinePtr ;

struct Token {
  CharPtr tokenStr ;
  TokenType type ;
  LinePtr firstAppearOn ;
  LinePtr lastAppearOn ;
  Token * next ;
} ; // Token

typedef Token * TokenPtr ;
typedef char Str400[400];
typedef char Str10[10];
typedef char Str20[20];
static int uTestNum = -1 ;

int gLine = 1 ;
int gColumn = 1 ;
int gTest = 0;
char gNextChar = '\0' ;
int gNextCharLine = -1 ;
int gNextCharColumn = -1 ;
bool gNotFinish = false;
bool gerror = false;
TokenPtr gFront = NULL ;

void GetNextChar( char & ch, int & line, int & column ) ;
CharPtr GetToken(  TokenPtr head, TokenType & tokenType, int & firstCharLine, int & firstCharColumn ) ;
void OrderInsertToken( TokenPtr & head, CharPtr tokenStr,
                       TokenType tokenType, int tokenLine, int tokenColumn ) ;

void PrintAllTokenInfo( int maxNumOfTokenToPrint, TokenPtr head ) ;
void HandleUI( TokenPtr listHead ) ;
void GetTokenInfo( TokenPtr head ) ;
void DeleteAllTokens( TokenPtr &head ) ;
bool Grammar( TokenPtr &listHead, map<CharPtr, CharPtr> definemp ) ;
bool ArithExp( TokenPtr &head, map<CharPtr, CharPtr> definemp ) ;
bool IDlessArithExpOrBexp( TokenPtr &head, map<CharPtr, CharPtr> definemp ) ;
bool NOT_IDStartArithOrExp( TokenPtr &head, map<CharPtr, CharPtr> definemp ) ;
bool Term( TokenPtr &head, map<CharPtr, CharPtr> definemp ) ;
bool Factor( TokenPtr &head, map<CharPtr, CharPtr> definemp ) ;
bool GetNextToken( TokenPtr &head ) ;
void UnexpToken( TokenPtr head ) ;
bool PeekNextToken( TokenPtr head, CharPtr &peekToken ) ;
bool BooleanOperator( TokenPtr &head ) ;
bool NOT_IDStartArithorBExp( TokenPtr &head, map<CharPtr, CharPtr> definemp ) ;
bool NOT_IDStartArithExp( TokenPtr &head, map<CharPtr, CharPtr> definemp ) ;
bool NOT_IDStartTerm( TokenPtr &head, map<CharPtr, CharPtr> definemp ) ;
bool NOT_IDStartFactor( TokenPtr &head, map<CharPtr, CharPtr> definemp ) ;
void UndefinedToken( TokenPtr head ) ;
void Reset( TokenPtr &head ) ;
bool CheckDefine( TokenPtr head, map<CharPtr, CharPtr> definemp ) ;
void DefineNewToken( TokenPtr head, map<CharPtr, CharPtr> definemp ) ;
void DefineNewID( TokenPtr head, map<CharPtr, CharPtr> &definemp ) ;
bool AfterIDCheck( TokenPtr head ) ;
bool Compare( double op1, double op2, Str20 compareOperator ) ;
CharPtr Execute( TokenPtr head, map<CharPtr, CharPtr> definemp  ) ;
void Infix2postfix( TokenPtr &head, vector< CharPtr >* postfix,
                     map<CharPtr, CharPtr> definemp, Str20 &compareOperator ) ;
int PrecedenceOperator( CharPtr x ) ;
bool IsOperator( CharPtr s ) ;
bool IsDecimal( Str10 s ) ;
bool Compare( Str10 num, Str10 num2, Str10 compareOperator ) ;
CharPtr GetValue( TokenPtr head, map<CharPtr, CharPtr> definemp ) ;

void GetNextChar( char & ch, int & line, int & column ) {
  scanf( "%c", &ch );
  line = gLine;
  column = gColumn;
  gColumn ++;
  if ( ch == '\n' ) {
    gLine ++;
    gColumn = 1;
  } // if
} // GetNextChar()

bool IsDigit( char ch ) {
  if ( ch >= 48 && ch <= 57 )  return true;
  else return false;
} // IsDigit()

bool IsLetter( char ch ) {
  if ( ( ch >= 65 && ch <= 90 ) || ( ch >= 97 && ch <= 122 ) )  return true;
  else   return false;
} // IsLetter()

bool IsWhiteSpace( char ch ) {
  if ( ch == ' ' || ch == '\n' || ch == '\t' )  return true;
  else return false;
} // IsWhiteSpace()

bool IsOthers( char ch ) {
  if ( ch == ':' || ch == '<' || ch == '>' || ch == '=' || ch == '(' || ch == ')' || ch == '*'
       || ch == '/' || ch == ';' )  return true;
  else return false;
} // IsOthers()

bool IsSign( char ch ) {
  if ( ch == '+' || ch == '-' )  return true;
  else return false;
} // IsSign()

void SkipWhiteSpaces() {
  while ( gNextChar  == ' ' || gNextChar == '\n' || gNextChar == '\t' ) {
    GetNextChar( gNextChar, gNextCharLine, gNextCharColumn ) ;
  } // while
} // SkipWhiteSpaces()

void SkipLineComment() {
  while ( gNextChar != '\n' ) {
    GetNextChar( gNextChar, gNextCharLine, gNextCharColumn ) ;
  } // while

  GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
} // SkipLineComment()

CharPtr GetID() {
  Str400 name;
  int i = 1;
  name[0] = gNextChar;
  GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
  while ( IsWhiteSpace( gNextChar ) == false && i < 399 &&
          ( IsDigit( gNextChar ) == true ||  IsLetter( gNextChar ) == true || gNextChar == '_' ) ) {
    name[i] = gNextChar;
    GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
    i ++;
  } // while

  name[i] = '\0';
  CharPtr tokenName = new char[i + 1];
  strcpy( tokenName, name ) ;
  return tokenName ;

} // GetID()

CharPtr GetNum() {
  Str400 name;
  int i = 1;
  bool isDot = false;
  name[0] = gNextChar;
  if ( gNextChar == '.' ) isDot = true;
  GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
  if ( name[0] == '.' && gNextChar == '.' ) return NULL;
  while ( !IsWhiteSpace( gNextChar ) && i < 399 &&
          ( IsDigit( gNextChar ) || ( gNextChar == '.' && !isDot ) ) ) {
    // '2...3' Unrecognized token with first char : '.'
    if ( gNextChar == '.' ) isDot = true;
    name[i] = gNextChar;
    GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
    i ++;
  } // while

  name[i] = '\0';
  CharPtr tokenName = new char[i + 1];
  strcpy( tokenName, name ) ;
  return tokenName ;
} // GetNum()

CharPtr GetOthers() {
  Str400 name;
  int i = 0;
  name[0] = gNextChar;
  GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
  if ( name[0] == ':' ) {
    if ( gNextChar != '=' )  return NULL;
    else {
      i ++;
      name[i] = gNextChar;
      GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
    } // else
  } // if
  else if ( name[0] == '<' ) {
    if ( gNextChar == '=' || gNextChar == '>' ) {
      i ++;
      name[i] = gNextChar;
      GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
    } // if
  } // else if
  else if ( name[0] == '>' ) {
    if ( gNextChar == '=' ) {
      i ++;
      name[i] = gNextChar;
      GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
    } // if
  } // else if

  name[i + 1] = '\0';
  CharPtr tokenName = new char[ i + 2 ];
  strcpy( tokenName, name ) ;
  return tokenName ;
} // GetOthers()

CharPtr GetToken( TokenPtr head, TokenType & tokenType, int & firstCharLine, int & firstCharColumn ) {
  Str400 name;
  Str10 c;
  CharPtr tokenName;
  // printf("%c\n",gNextChar);
  while ( IsWhiteSpace( gNextChar ) ||  gNextChar == '/'  ) {
    if ( IsWhiteSpace( gNextChar ) )  SkipWhiteSpaces() ;
    else if ( gNextChar == '/' ) {
      GetNextChar( gNextChar, gNextCharLine, gNextCharColumn ) ;
      if ( gNextChar == '/' )   SkipLineComment() ;
      else {
        c[0] = '/';
        c[1] = '\0';
        tokenName = new char[2];
        strcpy( tokenName, c );
        tokenType = OTHERS;
        return tokenName;
      } // else
    } // else if gNextChar is '/'
  } // while

  firstCharLine = gNextCharLine;
  firstCharColumn = gNextCharColumn;
  if ( IsLetter( gNextChar ) ) { // ignore first char with '_'
    tokenName = GetID();
    if ( strcmp( tokenName, "quit" ) == 0 && head == NULL ) {
      tokenType = QUIT;
    } // if
    else  tokenType = IDENTIFIER;
  } // if
  else if ( IsDigit( gNextChar ) || gNextChar == '.' ) {
    tokenName = GetNum() ;
    if ( tokenName == NULL || strcmp( tokenName, "." ) == 0 ) {
      c[0] = '.';
      c[1] = '\0';
      tokenName = new char[2];
      strcpy( tokenName, c );
      tokenType = UNRECOGNIZE;
      // printf("Unrecognized token with first char : '%c'\n", '.');
    } // if
    else tokenType = NUM;
  } // else if Digit
  else if ( IsOthers( gNextChar ) ) {
    tokenName = GetOthers() ;
    if ( tokenName == NULL ) {
      c[0] = ':';
      c[1] = '\0';
      tokenName = new char[2];
      strcpy( tokenName, c );
      tokenType = UNRECOGNIZE;
      // printf("Unrecognized token with first char : '%c'\n", ':');
    } // if
    else tokenType = OTHERS;
  } // else if OTHERS
  else if ( IsSign( gNextChar ) ) {
    c[0] = gNextChar;
    c[1] = '\0';
    tokenName = new char[2];
    GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
    strcpy( tokenName, c );
    tokenType = SIGN;
  } // else if
  else { // Unrecognized token!!!
    c[0] = gNextChar;
    c[1] = '\0';
    tokenName = new char[2];
    strcpy( tokenName, c );
    tokenType = UNRECOGNIZE;
    // printf("Unrecognized token with first char : '%c'\n", gNextChar);
    GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
  } // else

  return tokenName;

} // GetToken()

void OrderInsertToken( TokenPtr & head, CharPtr tokenStr,
                       TokenType tokenType, int tokenLine, int tokenColumn ) {

  TokenPtr tempt = head;
  TokenPtr temptoken = head;
  TokenPtr pre = NULL;
  ColumnPtr tempc = new Column;
  LinePtr tempL = NULL;
  temptoken = new Token;
  temptoken -> next = NULL;
  temptoken -> tokenStr = tokenStr;
  temptoken -> type = tokenType;

  tempL = new Line;
  tempL -> next = NULL;
  tempL -> line = tokenLine;

  tempc = new Column;
  tempc -> column = tokenColumn;
  tempc -> next = NULL;

  tempL -> firstAppearAt = tempc;
  tempL -> lastAppearAt = tempc;

  temptoken -> firstAppearOn = tempL;
  temptoken -> lastAppearOn = tempL;

  if ( head == NULL ) {
    head = temptoken;
  } // if

  else {
    while ( tempt != NULL ) {
      pre = tempt;
      tempt = tempt -> next;
    } // while

    pre -> next = temptoken;
  } // else

} // OrderInsertToken()


int main() {
  map<CharPtr, CharPtr> definemp;
  char ch = '\0' ;
  CharPtr tokenStr = NULL ;
  int tokenLine = 0, tokenColumn = 0 ;
  TokenType tokenType ;
  TokenPtr listHead = NULL ;   // !!! empty token list at the start
  scanf( "%d%c", &uTestNum, &ch ) ;
  GetNextChar( gNextChar, gNextCharLine, gNextCharColumn ) ;
  printf( "Program starts...\n" );


  do {
    tokenStr = GetToken( listHead, tokenType, tokenLine, tokenColumn ) ;
    OrderInsertToken( listHead, tokenStr, tokenType, tokenLine, tokenColumn ) ;
    if ( tokenType == UNRECOGNIZE ) { // token unrecognized
      if ( gNextChar != '\n' ) {
        scanf( "%c", &ch );
        while ( ch != '\n' )  scanf( "%c", &ch ); // read char till '\n'
      } // if

      printf( "> Unrecognized token with first char : '%s'\n", tokenStr );
      // GetTokenInfo( listHead );
      Reset( listHead );
      GetNextChar( gNextChar, gNextCharLine, gNextCharColumn ) ;

    } // if
    else if ( !Grammar( listHead, definemp ) &&  ( gerror || !gNotFinish ) ) {
      // GetTokenInfo( listHead );
      Reset( listHead );
      // printf("grammar error\n");
      GetNextChar( gNextChar, gNextCharLine, gNextCharColumn ) ;

    } // else if
    else if ( strcmp( tokenStr, ";" ) == 0  ) {
      // GetTokenInfo( listHead );
      if ( listHead -> next != NULL && strcmp( listHead -> next -> tokenStr, ":=" ) == 0 ) { // define id
        DefineNewID( listHead, definemp );
      } // if
      else {
        Execute( listHead, definemp );
      } // else

      // for( auto &s: definemp ) {
        // printf( "%s %s\n", s.first, s.second );
      // } // for

      // if ( !error ) printf( "grammar correct\n" );
      if ( gerror )  printf( "> Error\n" );
      Reset( listHead );
    } // else if

    if ( tokenType == QUIT ) {
      printf( "> Program exits..." );
      definemp.clear();
    } // if
  } while ( tokenType != QUIT ) ;

  return 0 ;

} // main()

CharPtr Execute( TokenPtr head, map<CharPtr, CharPtr> definemp ) {
  TokenPtr temp = head;
  int a = 0;
  double op1 = 0, op2 = 0, result = 0;
  double tempF = 0;
  bool isFloat = false, div = false;
  CharPtr return_value = new char[20];
  Str20 op1_string = {'\0'};
  Str20 op2_string = {'\0'};
  Str20 buffer = {'\0'};
  Str20 num = {'\0'};
  Str20 num2 = {'\0'};
  Str20 compareOperator = {'\0'};
  Str20 compareTemp = {'\0'};
  vector< CharPtr >* postfix;
  postfix = new vector< CharPtr >() ;
  stack< CharPtr >* calSt; // calculate stack
  calSt = new stack< CharPtr >();
  Infix2postfix( temp, postfix, definemp, compareTemp );
  strcpy( compareOperator, compareTemp );


  do { // 判斷是否為比較式
    if ( a == 1 ) Infix2postfix( temp -> next, postfix, definemp, compareTemp );
    for ( int i = 0 ; i < postfix -> size() ; i ++ ) {
      isFloat = false;
      div = false;
      if ( !IsOperator( postfix -> at( i ) ) ) calSt -> push( postfix -> at( i ) );
      else {
        if ( !calSt -> empty() ) {
          strcpy( op2_string, calSt -> top() );
          calSt -> pop();
        } // if
        else printf( "stack empty\n" );

        if ( !calSt -> empty() ) {
          strcpy( op1_string, calSt -> top() );
          calSt -> pop();
        } // if
        else printf( "stack empty\n" );

        if ( IsDecimal( op1_string ) || IsDecimal( op2_string ) ) isFloat = true;

        op1 = atof( op1_string );
        op2 = atof( op2_string );

        if ( strcmp( postfix -> at( i ), "+" ) == 0 ) result = op1 + op2;
        else if ( strcmp( postfix -> at( i ), "-" ) == 0 ) result = op1 - op2;
        else if ( strcmp( postfix -> at( i ), "*" ) == 0 ) result = op1 * op2;
        else if ( strcmp( postfix -> at( i ), "/" ) == 0 ) {
          div = true;
          if ( op2 == 0 ) {
            result = op1 / op2;
            gerror = true;
            return NULL;
          } // if
          else result = op1 / op2;
        } // else if

        if ( result < 0 && !isFloat && div ) { // div and 負數
          if ( floor( result ) != 0 ) result = floor( result * -1 );
          sprintf( buffer, "%1.0f", result );
        } // if
        else if ( isFloat )  {
          sprintf( buffer, "%1.3f", round( result * 1000.0 ) / 1000.0 );
          // calSt -> push( strdup( ss.str().c_str() ) );
        } // if
        else {
          sprintf( buffer, "%1.0f", floor( result ) );
          // calSt -> push( strdup( ss.str().c_str() ) );
        } // else

        char* dest = new char[ strlen( buffer ) + 1 ];
        strcpy( dest, buffer );
        calSt -> push( dest );
      } // else
    } // for

    if ( !calSt -> empty() ) {
      if ( a == 0 ) strcpy( num, calSt -> top() );
      else strcpy( num2, calSt -> top() );
      calSt -> pop();
    } // if
    // else printf( "error stack empty" );

    if ( !calSt -> empty() ) printf( "error stack not empty\n" );
    a++;
  } while ( strcmp( compareTemp, ";" ) != 0 ) ; // do-while

  delete postfix;
  delete calSt;
  if ( strcmp( compareTemp, ";" ) == 0 && a == 1 ) {
    if ( IsDecimal( num ) ) {
      sprintf( buffer, "%1.3f", round( atof( num ) * 1000.0 ) / 1000.0 );
      printf( "> %s\n", buffer );
      strcpy( return_value, buffer );
      return return_value;
    } // if
    else {
      printf( "> %d\n", atoi( num ) );
      strcpy( return_value, num );
      return return_value;
    } // else
  } // if
  else if ( a == 2 ) { // 比到小數點第四位
    op1 = round( atof( num ) * 10000.0 ) / 10000.0;
    op2 = round( atof( num2 ) * 10000.0 ) / 10000.0;
    if ( Compare( op1, op2, compareOperator ) ) printf( "> true\n" ) ;
    else printf( "> false\n" );
  } // else if

  return NULL;
} // Execute()

bool Compare( double op1, double op2, Str20 compareOperator ) {

  if ( strcmp( compareOperator, "=" ) == 0 ) {
    if ( op1 == op2 ) return true;
    else return false;
  } // if
  else if ( strcmp( compareOperator, "<>" ) == 0 ) {
    if ( op1 != op2 ) return true;
    else return false;
  } // else if
  else if ( strcmp( compareOperator, ">" ) == 0 ) {
    if ( op1 > op2 ) return true;
    else return false;
  } // else if
  else if ( strcmp( compareOperator, "<" ) == 0 ) {
    if ( op1 < op2 ) return true;
    else return false;
  } // else if
  else if ( strcmp( compareOperator, ">=" ) == 0 ) {
    if ( op1 >= op2 ) return true;
    else return false;
  } // else if
  else if ( strcmp( compareOperator, "<=" ) == 0 ) {
    if ( op1 <= op2 ) return true;
    else return false;
  } // else if

  return false;
} // Compare()

bool IsDecimal( Str20 s ) {
  for ( int i = 0 ; i < strlen( s ) ; i++ ) {
    if ( s[i] == '.' )  return true;
  } // for

  return false;
} // IsDecimal()

bool IsOperator( CharPtr s ) {
  if ( strcmp( s, "+" ) == 0 || strcmp( s, "-" ) == 0  ||
       strcmp( s, "*" ) == 0  || strcmp( s, "/" ) == 0  )  return true;
  else  return false;
} // IsOperator()

void Infix2postfix( TokenPtr &head, vector< CharPtr >* postfix,
                    map<CharPtr, CharPtr> definemp, Str20 &compareOperator ) {
  postfix -> clear();
  stack< CharPtr >* stackPtr;
  stackPtr = new stack< CharPtr >();
  while ( strcmp( head -> tokenStr, ";" ) != 0 && strcmp( head -> tokenStr, "=" ) != 0
          && strcmp( head -> tokenStr, "<>" ) != 0 && strcmp( head -> tokenStr, ">" ) != 0
          && strcmp( head -> tokenStr, "<" ) != 0 && strcmp( head -> tokenStr, ">=" ) != 0
          && strcmp( head -> tokenStr, "<=" ) != 0 ) {
    if ( head -> type == NUM ) {
      postfix -> push_back( head -> tokenStr );
    } // if
    else if ( head -> type == IDENTIFIER )  postfix -> push_back( GetValue( head, definemp ) );
    else if ( strcmp( head -> tokenStr, "(" ) == 0 || strcmp( head -> tokenStr, ")" ) == 0 ) {
      if ( strcmp( head -> tokenStr, "(" ) == 0 )  stackPtr -> push( head -> tokenStr );
      else {
        while ( strcmp( stackPtr -> top(), "(" ) != 0 ) {
          if ( !stackPtr -> empty() )  postfix -> push_back( stackPtr -> top() );
          stackPtr -> pop();
        } // while

        stackPtr -> pop(); // delete (
      } // else
    } // else if
    else { // If is an operator
      while ( !stackPtr -> empty() && PrecedenceOperator( head -> tokenStr )
              <= PrecedenceOperator( stackPtr -> top() ) ) {
        postfix -> push_back( stackPtr -> top() );
        stackPtr -> pop();
      } // while

      stackPtr -> push( head -> tokenStr );
    } // else

    head = head -> next;
  } // while

  strcpy( compareOperator, head -> tokenStr );

  while ( !stackPtr -> empty() ) { // pop all
    postfix -> push_back( stackPtr -> top() );
    stackPtr -> pop();
  } // while

  delete stackPtr;
} // Infix2postfix()

CharPtr GetValue( TokenPtr head, map<CharPtr, CharPtr> definemp ) {
  map<CharPtr, CharPtr>::iterator it ;
  for ( it = definemp.begin() ; it != definemp.end() ; ++it ) {
    if ( strcmp( it -> first, head -> tokenStr ) == 0 ) return it -> second;
  } // for

  return NULL;
} // GetValue()

int PrecedenceOperator( CharPtr x ) {
  if ( x[0] == '+' || x[0] == '-' )  return 1;
  else if ( x[0] == '/' || x[0] == '*' )  return 2;
  return 0;
} // PrecedenceOperator()

void DefineNewID( TokenPtr head, map<CharPtr, CharPtr> &definemp ) {
  CharPtr value = NULL;
  map<CharPtr, CharPtr>::iterator it ;
  CharPtr buffer = NULL;
  value = Execute( head -> next -> next, definemp );
  if ( value == NULL ) {
    gerror = true;
    return;
  } // if

  for ( it = definemp.begin() ; it != definemp.end() ; ++it ) {
    if ( strcmp( it -> first, head -> tokenStr ) == 0 ) {
      strcpy( it -> second, value );
      return;
    } // if
  } // for

  buffer = new char[ strlen( head -> tokenStr ) + 1 ];
  strcpy( buffer, head -> tokenStr );
  definemp[ buffer ] = value;
} // DefineNewID()

void Reset( TokenPtr &head ) {
  DeleteAllTokens( head );
  gLine = 1 ;
  gColumn = 1 ;
  gNotFinish = false;
  gerror = false;
} // Reset()

bool CheckDefine( TokenPtr head, map<CharPtr, CharPtr> definemp ) {
  map<CharPtr, CharPtr>::iterator it ;
  for ( it = definemp.begin() ; it != definemp.end() ; ++it ) {
    if ( strcmp( it -> first, head -> tokenStr ) == 0 )  return true;
  } // for

  return false;
} // CheckDefine()

bool AfterIDCheck( TokenPtr head ) {
  CharPtr peekToken = NULL;
  if ( !PeekNextToken( head, peekToken ) )  return false; // 檢查id下一位是否為合法token
  if ( strcmp( peekToken, "+" ) == 0 || strcmp( peekToken, "-" ) == 0 || strcmp( peekToken, "*" ) == 0
       || strcmp( peekToken, "/" ) == 0 || strcmp( peekToken, "=" ) == 0 || strcmp( peekToken, "<>" ) == 0
       || strcmp( peekToken, ">" ) == 0 || strcmp( peekToken, "<" ) == 0 || strcmp( peekToken, "<=" ) == 0
       || strcmp( peekToken, ">=" ) == 0 || strcmp( peekToken, ";" ) == 0 )  return true;
  else  return false;
} // AfterIDCheck()

bool Grammar( TokenPtr &listHead, map<CharPtr, CharPtr> definemp ) {
  TokenPtr head = listHead;
  CharPtr peekToken = NULL;
  if ( head -> type == QUIT ) return true;

  if ( head -> type == IDENTIFIER ) { // IDENT ( ':=' <ArithExp> | <IDlessArithExpOrBexp>)
    if ( !PeekNextToken( head, peekToken ) )  return false;
    if ( strcmp( peekToken, ":=" ) == 0 ) {  // IDENT ':=' <ArithExp>
      if ( !GetNextToken( head ) )  return false; // get ":="
      else if ( !GetNextToken( head ) )  return false;
      else if ( !ArithExp( head, definemp ) )  return false;
      // IDENT ':=' <ArithExp>
    } // if
    else { // 判斷有沒有宣告過
      // 此token之後是不合於文法的token (e.g., '('或')')
      // 錯誤出在「此token(即ID)之後的(不合於文法的)token」(e.g., '('或')')
      if ( !AfterIDCheck( head ) ) {
        UnexpToken( head -> next );
        return false;
      } // if
      else if ( !CheckDefine( head, definemp ) ) {
      // 此token(即ID)之後是合於文法的token (e.g., '+'或'-') 此token(即ID)必須被define過
        UndefinedToken( head );
        return false;
      } // else if

      if ( !IDlessArithExpOrBexp( head, definemp ) )  return false; // IDENT  ':=' <IDlessArithExpOrBexp>
      else { // IDENT ':=' <IDlessArithExpOrBexp>
        if ( !GetNextToken( head ) )  return false;
        if ( strcmp( head -> tokenStr, ";" ) == 0 )  return true; // 可能只有id;
      } // else
    } // else
  } // if ID Start
  else { // not define ID
    if ( !NOT_IDStartArithorBExp( head, definemp ) )  return false;
  } // else

  if ( !GetNextToken( head ) )  return false;
  else if ( strcmp( head -> tokenStr, ";" ) == 0 )  return true;
  else {
    UnexpToken( head );
    return false;
  } // else
} // Grammar()

bool ArithExp( TokenPtr &head, map<CharPtr, CharPtr> definemp ) {
  CharPtr peekToken = NULL;
  if ( !Term( head, definemp ) )  return false;
  else {
    if ( !PeekNextToken( head, peekToken ) )  return false;
    while ( peekToken[0] == '+' || peekToken[0] == '-' ) { // peek next token
      if ( !GetNextToken( head ) )  return false; // get '+' or '-'
      if ( !GetNextToken( head ) )  return false; // get token to run Term
      if ( !Term( head, definemp ) ) return false;
      if ( !PeekNextToken( head, peekToken ) )  return false;
    } // while
  } // else

  return true;
} // ArithExp()

bool IDlessArithExpOrBexp( TokenPtr &head, map<CharPtr, CharPtr> definemp ) {
  CharPtr peekToken = NULL;
  if ( !PeekNextToken( head, peekToken ) )  return false;
  while ( peekToken[0] == '+' || peekToken[0] == '-' || peekToken[0] == '*' || peekToken[0] == '/' ) {
    if ( !GetNextToken( head ) )  return false; // get '+' or '-' or '*' or '/'
    if ( !GetNextToken( head ) )  return false;
    if ( peekToken[0] == '+' || peekToken[0] == '-' ) {
      if ( !Term( head, definemp ) )  return false;
    } // if
    else { // peekToken[0] == '*' || peekToken[0] == '/'
      if ( !Factor( head, definemp ) )  return false;
    } // else
  } // while

  if ( BooleanOperator( head ) ) {
    if ( !GetNextToken( head ) )  return false;
    if ( !ArithExp( head, definemp ) )  return false;
  } // if

  return true;
} // IDlessArithExpOrBexp()

bool NOT_IDStartArithorBExp( TokenPtr &head, map<CharPtr, CharPtr> definemp ) {
  if ( !NOT_IDStartArithExp( head, definemp ) )  return false;
  if ( BooleanOperator( head ) ) {
    if ( !GetNextToken( head ) )  return false;
    if ( !ArithExp( head, definemp ) )  return false;
  } // if

  return true;
} // NOT_IDStartArithorBExp()

bool NOT_IDStartArithExp( TokenPtr &head, map<CharPtr, CharPtr> definemp ) {
  CharPtr peekToken = NULL;
  if ( !NOT_IDStartTerm( head, definemp ) )  return false;
  if ( !PeekNextToken( head, peekToken ) )  return false;
  while ( peekToken[0] == '+' || peekToken[0] == '-' ) { // peek next token
    if ( !GetNextToken( head ) )  return false; // get '+' or '-'
    if ( !GetNextToken( head ) )  return false; // get token to run Term
    if ( !Term( head, definemp ) ) return false;
    if ( !PeekNextToken( head, peekToken ) )  return false;
  } // while

  return true;
} // NOT_IDStartArithExp()

bool NOT_IDStartTerm( TokenPtr &head, map<CharPtr, CharPtr> definemp ) {
  CharPtr peekToken = NULL;
  if ( !NOT_IDStartFactor( head, definemp ) )  return false; // get a token to judge
  if ( !PeekNextToken( head, peekToken ) )  return false;
  while ( peekToken[0] == '*' || peekToken[0] == '/' ) { // peek next token
    if ( !GetNextToken( head ) )  return false; // get next token  '*' or '/'
    // 看是否為<Factor>
    if ( !GetNextToken( head ) )  return false; // get token to run Factor
    if ( !Factor( head, definemp  ) )  return false;
    if ( !PeekNextToken( head, peekToken ) )  return false;
  } // while

  return true;
} // NOT_IDStartTerm()

bool NOT_IDStartFactor( TokenPtr &head, map<CharPtr, CharPtr> definemp ) {
  TokenPtr pre = NULL;
  CharPtr combine = new char[20];
  string* str1 = new string;
  if ( strcmp( head -> tokenStr, "+" ) == 0 ||
       strcmp( head -> tokenStr, "-" ) == 0  || head -> type == NUM ) {
    if ( strcmp( head -> tokenStr, "+" ) == 0 ||
         strcmp( head -> tokenStr, "-" ) == 0 ) { // combine sign num as a num
      pre = head;
      if ( !GetNextToken( head ) )   return false;
      if ( head -> type == NUM ) {
        pre -> next = head -> next;
        pre -> type = NUM;
        *str1 = pre -> tokenStr;
        str1 -> append( head -> tokenStr );
        delete[] pre->tokenStr;
        pre -> tokenStr = new char[str1 -> length() + 1];
        strcpy( pre -> tokenStr, str1 -> c_str() );
        // sprintf( combine, "%s%s", pre -> tokenStr, head -> tokenStr ); // combine sign and num ex. +1 -1

        delete head;  // 釋放舊的 head 所指向的記憶體
        head = pre;
        return true; // sign num
      } // if
      else {
        UnexpToken( head );
        return false;
      }  // else
    } // if
    else  return true; // num
  } // if
  else if ( strcmp( head -> tokenStr, "(" ) == 0 ) { // '('<ArithExp> ')'
    if ( !GetNextToken( head ) )  return false;
    if ( !ArithExp( head, definemp ) )  return false; // not <ArithExp>
    else { // '('<ArithExp>
      if ( !GetNextToken( head ) )  return false; // Unrecognized token
      if ( strcmp( head -> tokenStr, ")" ) == 0 )  return true;
      else {
        UnexpToken( head );
        return false;
      } // else
    } // else
  } // else if
  else {
    UnexpToken( head );
    return false;
  } // else
} // NOT_IDStartFactor()

bool BooleanOperator( TokenPtr &head ) {
  CharPtr peekToken = NULL;
  if ( !PeekNextToken( head, peekToken ) )  return false;
  if ( strcmp( peekToken, "=" ) == 0 || strcmp( peekToken, "<>" ) == 0 || strcmp( peekToken, ">" ) == 0 ||
       strcmp( peekToken, "<" ) == 0 || strcmp( peekToken, ">=" ) == 0 || strcmp( peekToken, "<=" ) == 0 ) {
    if ( !GetNextToken( head ) )  return false;
    return true;
  } // if
  else  return false; // not BooleanOperator 不一定要出現，不用unexp
} // BooleanOperator()

bool Term( TokenPtr &head, map<CharPtr, CharPtr> definemp ) {
  // <Term> ::= <Factor> { '* <Factor> | '/' <Factor>}
  CharPtr peekToken = NULL;
  if ( !Factor( head, definemp ) )  return false;
  else {
    if ( !PeekNextToken( head, peekToken ) )  return false;
    while ( peekToken[0] == '*' || peekToken[0] == '/' ) { // peek next token
      if ( !GetNextToken( head ) )  return false; // get next token  '*' or '/'
      // 看是否為<Factor>
      if ( !GetNextToken( head ) )  return false; // get token to run Factor
      if ( !Factor( head, definemp  ) )  return false;
      if ( !PeekNextToken( head, peekToken ) )  return false;
    } // while
  } // else

  return true;
} // Term()

bool Factor( TokenPtr &head, map<CharPtr, CharPtr> definemp ) {
  string* str1 = new string;
  // <Factor> ::= IDENT | [SIGN] NUM |'('<ArithExp> ')'
  TokenPtr pre = NULL;
  CharPtr combine = new char[20];
  if ( head -> type == IDENTIFIER ) {
    if ( !CheckDefine( head, definemp ) ) {
      UndefinedToken( head );
      return false;
    } // if

    return true;
  } // if
  else if ( strcmp( head -> tokenStr, "+" ) == 0 || strcmp( head -> tokenStr, "-" ) == 0
            || head -> type == NUM ) {
    if ( strcmp( head -> tokenStr, "+" ) == 0 || strcmp( head -> tokenStr, "-" ) == 0 ) {
      // combine sign num as a num
      pre = head;
      if ( !GetNextToken( head ) )   return false;
      if ( head -> type == NUM ) {
        pre -> next = head -> next;
        pre -> type = NUM;
        *str1 = pre -> tokenStr;
        str1 -> append( head -> tokenStr );
        delete[] pre->tokenStr;
        pre -> tokenStr = new char[str1 -> length() + 1];
        strcpy( pre -> tokenStr, str1 -> c_str() );
        delete head;  // 釋放舊的 head 所指向的記憶體
        head = pre;
        return true; // sign num
      } // if
      else {
        UnexpToken( head );
        return false;
      }  // else
    } // if
    else  return true; // num
  } // else if
  else if ( strcmp( head -> tokenStr, "(" ) == 0 ) { // '('
    if ( !GetNextToken( head ) )  return false;
    if ( !ArithExp( head, definemp ) )  return false; // not <ArithExp>
    else { // '('<ArithExp>
      if ( !GetNextToken( head ) )  return false; // Unrecognized token
      if ( strcmp( head -> tokenStr, ")" ) == 0 )  return true;
      else {
        UnexpToken( head );
        return false;
      } // else
    } // else
  } // else if
  else {
    UnexpToken( head );
    return false;
  } // else
} // Factor()

bool PeekNextToken( TokenPtr head, CharPtr &peekToken ) {
  if ( head -> next != NULL ) {
    peekToken = head -> next -> tokenStr;
    return true;
  } // if
  else {
    gNotFinish = true;
    return false;
  } // else
}  // PeekNextToken()

bool GetNextToken( TokenPtr &head ) {
  char ch = '\0' ;
  if ( head-> next != NULL ) head = head -> next;
  else { // head->next = NULL
    gNotFinish = true;
    return false;
  } // else

  return true;
} // GetNextToken()

void UnexpToken( TokenPtr head ) {
  char ch = '\0';
  gerror = true;
  printf( "> Unexpected token : \'%s\'\n", head ->tokenStr );
  if ( gNextChar != '\n' ) {
    scanf( "%c", &ch );
    while ( ch != '\n' )  scanf( "%c", &ch ); // read char till '\n'
  } // if
} // UnexpToken()

void UndefinedToken( TokenPtr head ) {
  char ch = '\0';
  gerror = true;
  printf( "> Undefined identifier : \'%s\'\n", head ->tokenStr );
  if ( gNextChar != '\n' ) {
    scanf( "%c", &ch );
    while ( ch != '\n' )  scanf( "%c", &ch ); // read char till '\n'
  } // if
} // UndefinedToken()

void GetTokenInfo( TokenPtr head ) {
  while ( head != NULL ) {
    ColumnPtr tempC = head -> firstAppearOn -> firstAppearAt;
    LinePtr tempL = head -> firstAppearOn ;
    printf( "%s", head -> tokenStr ) ;
    while ( tempL != NULL ) {
      while ( tempC != NULL ) {
        printf( " (%d,%d)", tempL -> line, tempC -> column );
        tempC = tempC -> next;
      } // while

      tempL = tempL -> next;
      if ( tempL != NULL )  tempC = tempL -> firstAppearAt;
    } // while

    head = head->next;
  } // while

  printf( "\n" );
} // GetTokenInfo()

void DeleteAllTokens( TokenPtr & head ) {
  while ( head != NULL ) {
    TokenPtr temp = head;
    head = head -> next; // 移動到下一個令牌
    delete[] temp -> tokenStr; // 釋放令牌字符串內存
    temp -> tokenStr = NULL;
    LinePtr lineTemp = temp -> firstAppearOn;
    while ( lineTemp != NULL ) {
      ColumnPtr columnTemp = lineTemp -> firstAppearAt;
      while ( columnTemp != NULL ) {
        ColumnPtr columnToDelete = columnTemp;
        columnTemp = columnTemp -> next;
        delete columnToDelete; // 釋放列內存
        columnToDelete = NULL;
      } // while

      LinePtr lineToDelete = lineTemp;
      lineTemp = lineTemp -> next;
      delete lineToDelete; // 釋放行內存
      lineToDelete = NULL;
    } // while

    delete temp; // 釋放令牌內存
    temp = NULL;
  } // while
} // DeleteAllTokens()


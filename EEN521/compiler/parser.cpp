// this is parser.cpp
#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include "useful.h"
#include "reader.h"
#include "parser.h"

using namespace std;

int str_ind = 0;

node * parse_statement( reader & input )
 { string s;
   input >> s;
   if (s == "print")
    { node *n = N("print");
      n->add(parse_expression(input));
      return n;	}

   if (s == "char")
   {
	   input.putbacksymbol();
	   node *n = parse_expression(input);
       input >> s;
	   if(s != "="){
		   input.error("missing assignment for char statement");
	   }
	   n->add(parse_expression(input));
	   return n;
   }

   if (s == "printchar")
    { node *n = N("printchar");
      n->add(parse_expression(input));
      return n;	}

   if (s == "printstr")
    { node *n = N("printstr");
      n->add(parse_expression(input));
      return n;	}

   if (s == "halt")
      return N("halt");

   if (s=="while")
    { node * n = N("while");
      n->add(parse_expression(input));
      input >> s;
      if (s!="do")
         input.error("no do after while");
      n->add(parse_statement(input));
      return n; }

   if (s=="local" || s=="global")
    { node * n = N(s);
      while ( true )
       { input >> s;
         if ( !isalpha(s[0]) )
            input.error("not a valid name");
         node * v = N("item", s);
         input >> s;
         if ( s == ":" )
          { input >> s;
            if (isalpha(s[0])) {
                v->ref_var = s;
                v->value = -1;
                input >> s;
            }
            else {
                bool ok = string_to_int(s, v->value);
                if (!ok)
                    input.error("not a number following ':'");
                input >> s; } }
         n->add(v);
         if ( s != "," )
          { input.putbacksymbol();
            break; } }
      return n; } 
    
    if (s == "const")
        return parse_const(input);
   
   if (s == "*") {
	   node *n = N("assign_ptr");
	   input >> s;

	   if (s == "(") {
		   input >> s;
		   n->add(parse_expression(input));
		   input >> s;
		   if (s != ")")
			   input.error("pointer reference has syntactical error, no trailing ')'");
	   }
	   else {
           input.putbacksymbol();
		   node *tmp = parse_expression(input);
		   if (tmp->tag != "variable") input.error("invalid pointer reference");
		   n->add(tmp);
	   }

	   input >> s;
	   if (s != "=") input.error("began with reference to var, but no assignment");
	   n->add(parse_expression(input));
	   return n;
   }

   if (s=="return")
    { node *n = N("return");
      input >> s;
      if (s != "}")
      { input.putbacksymbol();
        n->add(parse_expression(input)); }
      else
      { input.putbacksymbol();
        n->detail = "void"; }
      return n; }

    if (s=="if")
     { node *n = N("if");
       n->add(parse_expression(input));
       input >> s;
       if (s != "then")
         input.error("no 'then' after 'if'");
       n->add(parse_statement(input));
       input >> s;
       if (s == "else")
        { n->detail = "else";
          input.putbacksymbol();
          n->add(parse_else(input)); }
       else 
         input.putbacksymbol();
       return n; }

    if (s=="else")
      input.error("Invalid else statement without preceding if statement"); 

    if (s == "call")
    { node *n = N("functioncall");
	  int num_args = 0;
      input >> s;
      if ( !isalpha(s[0]) )
          input.error("not a valid name");
      n->detail = s;
      input >> s;
      if ( s != "(")
          input.error("missing a ( from the expression");
      while(true){
         input >> s;
         if ( !isalpha(s[0]) && s != ")" && !isdigit(s[0]))
            input.error("not a name or a ) statement");
         if (s == ")")
            break;
         else{
            input.putbacksymbol();
            n->add(parse_expression(input));
			num_args++;
         }
         input >> s;
         if( s != "," && s != ")")
            input.error("parameters did not end properlly.");
         if( s == ")")
            break;
      }
	  n->value = num_args;
      return n; 
    }

	if (s == "<|") {
		s = input.read_assembly();
		return N("assembly", s);
	}

    if (isalpha(s[0]))
     { node * n = N("assignment");
       n->add(N("variable", s));
       input >> s;
       if (s!="=")
         input.error("began with var but not assignment");
       n->add(parse_expression(input));
       return n; }

   if (s == "{")
    { node * result = N("sequence");
      s = ";";
      while (s == ";")
       { result -> add(parse_statement(input));
         input >> s; }
      if (s != "}")
       { input.error("No '}' - probably mistake in previous statement ");
         exit(1); }
      return result; }

   if (s == "[")
    { string tag, detail = "";
      int value = 0;
      input >> tag;
      input >> s;
      if (s == ":")
         input >> detail >> s;
      if (s == "=")
       { input >> s;
         value = string_to_int(s);
         input >> s; }
      node *result = N(tag, detail, value);
      while (s == ",")
       { result -> add(parse_statement(input)); 
         input >> s; }
      if (s != "]")
       { cerr << "ERROR No ]" << endl;
         exit(1); }
         return result; }

   input.error("Invalid beginning for a statement"); }

node * parse_else( reader & input )
 { string s;
   input >> s;
   if (s=="else")
    { node *n = N("else");
      n->add(parse_statement(input));
      return n; }
   input.error("Invalid else statement without preceding if statement"); }

node * parse_block( reader & input )
 { string s;
   input >> s;
   if (s != "{")
     input.error("Block required, but no '{'");
   node * result = N("sequence");
   s = ";";
   while (s == ";")
    { result -> add(parse_statement(input));
      input >> s; }
   if (s != "}")
     input.error("No '}' - probably mistake in previous statement");
   return result; }

node * parse_top_level( reader & input )
{  string s;
   input >> s;

   if (s == "function")
    { node *n = N("functiondef");
      int num_args = 0;
      input >> s;
      if ( !isalpha(s[0]) )
          input.error("not a valid name");
      n->detail = s;
      input >> s;
      if ( s != "(")
          input.error("missing a ( from the expression");
      while(true){
         input >> s;
         if ( (!isalpha(s[0])) && (s != ")"))
            input.error("not a name or a )");
         if (s == ")")
            break;
         else{
            node *v = N("variable",s);
            n->add(v);
			num_args++;
         }
         input >> s;
         if( s != "," && s != ")")
            input.error("parameters did not end properlly.");
         if( s == ")")
            break;
      }
	  n->value = num_args;
      n->add(parse_block(input));
      return n; }

   if (s == "global")
    { node * n = N(s);
      while ( true )
      {  input >> s;
         if ( !isalpha(s[0]) )
            input.error("not a valid name");
		 if (s == "MEMSTART" || s == "MEMAVAIL")
			input.error("cannot use reserved name as variable name");
         node * v = N("item", s);
         input >> s;
         if ( s == ":" )
          { input >> s;
            if (isalpha(s[0])) {
                v->ref_var = s;
                v->value = -1;
                input >> s;
            }
            else {
                bool ok = string_to_int(s, v->value);
                if (!ok)
                    input.error("not a number following ':'");
                input >> s; } }
         n->add(v);
         if ( s != "," )
          { input.putbacksymbol();
            break; } 
      }
      input >> s;
      return n; 
   }

    if (s == "export") {
       input >> s;
       if (s != "function" && s != "global") {
           cout << "Error, invalid export: \"" << s << "\"\n";
           exit(0);
       }
       node *n = N("export", s);
       
       while(true) {
          input >> s;
          n->add(N("item", s));
          input >> s;
          if (s != ",") break;
       }
       return n;
    }

    if (s == "const")
        return parse_const(input);

   if (s == "main")
    { node * result = N("main");
      input >> s;
      if (s != "{")
        input.error("main must be followed by '{'");
      input.putbacksymbol();
      result->add(parse_block(input));
      return result; }


   if (s == "end")
      return N("end");

   input.error("Invalid beginning for a top level def");
}

node * parse_const(reader & input)
{
    string s;
    node *n = N("const");

    while (true) {
        input >> s;
        if (!isalpha(s[0])) input.error("invalid const name");

        node *tmp = N("item", s);

        input >> s;
        if (s != "=") input.error("invalid const declaration");
        input >> s;
        tmp->value = string_to_int(s);
        n->add(tmp);
        input >> s;
        if (s == ",") continue;
        else if (s == ";") break;
        else input.error("const statement did not end properly");
    }
    return n;
}

node * parse(reader & input)
{
	node *prog = N("program");
	node *tmp = prog;

	while (tmp->tag != "end")
	{
		tmp = parse_top_level(input);
		tmp->add_top_level_decl();
	}
	tmp = N("global"); 
	tmp->add(N("item", "MEMSTART"));
	tmp->add(N("item", "MEMAVAIL"));
	tmp->add_top_level_decl();
	prog->add(tmp);

	tmp = prog;
	input.reset();
	while (tmp->tag != "end")
	{
		tmp = parse_top_level(input);
		prog->add(tmp);
	}

	return prog;
}

node * parse_expression( reader & input )
 { string s;
   input >> s;
   if (isdigit(s[0]))
    { int value = string_to_int(s);
      return N("number", "", value); }

   if (s == "char")
   {
	   node *n = N("stringindex");
	   n->add(parse_expression(input)); 
       input >> s;
	   if(s != "of"){
		   input.error("missing an of for the char");
	   }
       input >> s;
	   if(!isalpha(s[0]))
	   {
		   input.error("invalid string name");
	   }
       input.putbacksymbol();
	   n->add(parse_expression(input));
	   return n;
   }

   if (s == "call")
   {  node *n = N("functioncall");
	  int num_args = 0;
      input >> s;
      if ( !isalpha(s[0]) )
          input.error("not a valid name");
      n->detail = s;
      input >> s;
      if ( s != "(")
          input.error("missing a ( from the expression");
      while(true){
         input >> s;
         if ( !isalpha(s[0]) && s != ")" && !isdigit(s[0]))
            input.error("not a name or a )");
         if (s == ")")
            break;
         else{
            input.putbacksymbol();
            n->add(parse_expression(input));
			num_args++;
         }
         input >> s;
		 if( s != "," && s != ")") 
            input.error("parameters did not end properly.");
         if( s == ")")
            break;
      }
	  n->value = num_args;
      return n;
    }

   if (s == "*") {
	   node *n = N("follow_ptr");
	   input >> s;

	   if (s == "(") {
		   input >> s;
		   n->add(parse_expression(input));
		   input >> s;
		   if (s != ")")
			   input.error("pointer reference has syntactical error, no trailing ')'");
	   }
	   else {
           input.putbacksymbol();
		   node *tmp = parse_expression(input);
		   if (tmp->tag != "variable") input.error("invalid pointer reference");           
		   n->add(tmp);
	   }
	   return n;
   }

   if (s == "&") {
	   node *n = N("get_ptr");
	   node *tmp = parse_expression(input);
	   if (tmp->tag != "variable") input.error("invalid pointer reference");
	   n->add(tmp);
	   return n;
   }

   if (s == "(")
    { node * n = N("binop");
      n->add(parse_expression(input));
      input >> n->detail;
      n->add(parse_expression(input));
      input >> s;
      if (s != ")")
         input.error("No ')' - error in preceding expression");
      return n;	}

   if (s[0] == '\"' && s[s.length() - 1] == '\"') {
      stringstream tmp_stream;
      tmp_stream << "string_" << (str_ind++)/2;
      node *n = N("string", s);
      n->ref_var = tmp_stream.str();
	  return n;
   }
   if (s == "inchar")
	  return N("inchar");

   if (isalpha(s[0]))
      return N("variable", s);

   if (s == "[")
    { string tag, detail = "";
      int value = 0;
      input >> tag;
      input >> s;
      if (s == ":")
         input >> detail >> s;
      if (s == "=")
       { input >> s;
         value = string_to_int(s);
         input >> s; }
      node *result = N(tag, detail, value);
      while (s == ",")
       { result -> add(parse_expression(input)); 
         input >> s; }
      if (s != "]")
       { cerr << "ERROR No ]" << endl;
         exit(1); }
      return result; }
	
  input.error("Invalid beginning for an expression"); }

node * node::add( node * n )
 { part.push_back(n);
   return this;	}

void node::print( int indent )
 { cout << "[" << tag;
   indent += 1+tag.length();
   if (detail != "")
    { cout << ":" << detail;
      indent += 1+detail.length(); }
   if (value != 0)
    { cout << "=" << value;
      indent += 1+inttostring(value).length(); }
   if (ref_var != "")
    { cout << ";" << ref_var;
      indent += 1+ref_var.length(); }
   for (int i=0; i < part.size(); i+=1)
    { cout << ",\n";
      for (int j=0; j<indent; j++)
         cout << " ";
      part[i]->print(indent+1);	}
   cout << "]"; }

node * N( string t, string d, int v )
 { node *n = new node;
   n->tag = t;
   n->detail = d;
   n->value = v;
   return n; }

   
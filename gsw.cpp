//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// gsw
// Copyright 2015 Gabor T. Marth, University of Utah
// All rights reserved.
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

// standard includes
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iterator>
#include <algorithm>
#include <cmath>

// "tclap" commandline parsing library
#include <tclap/CmdLine.h>

// "boost" regular expression library
#include <boost/regex.hpp>
#include <boost/format.hpp>

// include classes
#include "Class-GraphAlignment.h"

// uses
using namespace std; 
using namespace TCLAP; 

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// templates
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// subroutines
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// typedefs
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

struct ArgStruct {
  string shortId;
  string longId;
  string description;
  bool required;
  string defaultValueString;
  int defaultValueInt;
  double defaultValueDouble;
  bool defaultValueBool;
  string type;
  bool multi;
  vector<string> constraint;
};

struct AlleleData {
  string base;
  short qual;
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// static variables
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

static string ProgramName("gsw");
static string ProgramDescription("Smith-Waterman optimal alignment algorithm");
static string ProgramVersion("0.4.1");
static string ProgramDate("2015-04-18");
static string ProgramDeveloper("Gabor T. Marth");
static string ProgramInstitution("University of Utah");
static string ProgramCopyrightDates("2015");

static vector<ArgStruct> ArgList;

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// classes
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

class MyOutput : public StdOutput {
public:
  
  virtual void failure(CmdLineInterface& c, ArgException& e)
  {
    cerr << "################################################################################" << endl;
    cerr << "### Program: " << ProgramName << " Version: " <<  ProgramVersion << " Release date: " << ProgramDate << endl;
    cerr << "### " << ProgramDescription << endl;
    cerr << "### " << "Copyright: " << ProgramCopyrightDates << " " << ProgramDeveloper << ", " << ProgramInstitution << endl;
    cerr << "### All rights reserved." << endl;
    cerr << "###" << endl;
    cerr << "### Command line error:" << e.what() << endl;
    cerr << "### For usage please type: " << c.getProgramName() << " --help" << endl;
    cerr << "################################################################################" << endl;
  }
  
  virtual void usage(CmdLineInterface& c)
  {

    cout << "################################################################################" << endl;
    cout << "### Program: " << ProgramName << " Version: " <<  ProgramVersion << " Release date: " << ProgramDate << endl;
    cout << "### " << ProgramDescription << endl;
    cout << "### " << "Copyright: " << ProgramCopyrightDates << " " << ProgramDeveloper << ", " << ProgramInstitution << endl;
    cout << "### All rights reserved." << endl;
    cout << "###" << endl;
    cout << "### Usage: " << c.getProgramName() << " [arguments], where:" << endl;
    for(vector<ArgStruct>::const_iterator it = ArgList.begin(); 
	it != ArgList.end(); it++) {
      ArgStruct arg = *it;

      string idString = "";
      if (arg.longId != "") {idString += "--" + arg.longId;}
      if (arg.shortId != "") {idString += ", -" + arg.shortId;}

      string multiString = "single-valued";
      if (arg.multi) {multiString = "multi-valued";}

      if (arg.required) {
	cout << "### " << idString << " [" << arg.type << ", required, no default, " << multiString << "]" << endl;
      }
      else {
	cout << "### " << idString << " [" << arg.type << ", optional, default=" << arg.defaultValueString << ", " << multiString << "]" << endl;
      }
      if (arg.constraint.size() > 0) {
	cout << "###     Permitted values: (";
	bool first = true;
	for (vector<string>::const_iterator iter = arg.constraint.begin();
	     iter != arg.constraint.end(); iter++) {
	  string value = *iter;
	  if (! first) {
	    cout << "|";
	  } 
	  first = false;
	  cout << value;
	}
	cout << ")" << endl;
      }
      cout << "###     Description: " << arg.description << endl;
    }
    cout << "################################################################################" << endl;
  }
  
  virtual void version(CmdLineInterface& c)
  {
    cerr << "################################################################################" << endl;
    cerr << "### Program: " << ProgramName << " Version: " <<  ProgramVersion << " Release date: " << ProgramDate << endl;
    cout << "### " << ProgramDescription << endl;
    cout << "### " << "Copyright: " << ProgramCopyrightDates << " " << ProgramDeveloper << ", " << ProgramInstitution << endl;
    cout << "### All rights reserved." << endl;
    cout << "###" << endl;
    cerr << "################################################################################" << endl;
  }
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

GraphAlignment *updateGA(vector<Node *> subjectNodes, string query, int M, int X, int GI, int GE, bool debug){
  GraphAlignment * ga;
  ga = new GraphAlignment(subjectNodes, query, M, X, GI, GE, debug);
  return ga;
}

int main (int argc, char *argv[]) {

  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------
  // constants
  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------
  static int D = 0;
  static int H = 1;
  static int V = 2;


  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------
  // command line
  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  // Create new CmdLine object
  //----------------------------------------------------------------------------
  CmdLine cmd("", ' ', ProgramVersion);
    
  //----------------------------------------------------------------------------
  // add program-specific command line arguments
  //----------------------------------------------------------------------------

  // initialize arg
  ArgStruct arg;

  // subject
  ArgStruct argSubject;
  arg = argSubject; 
  arg.shortId = "s"; 
  arg.longId = "subject"; 
  arg.description = "Subject sequence";
  arg.required = false; 
  arg.defaultValueString ="ATCCAGTAGGGATCCAT"; 
  arg.type = "string"; 
  arg.multi = false; 
  ArgList.push_back(arg);
  ValueArg<string> cmd_subject(arg.shortId, arg.longId, arg.description, arg.required, arg.defaultValueString, arg.type, cmd);

  // query
  ArgStruct argQuery;
  arg = argQuery; 
  arg.shortId = "q"; 
  arg.longId = "query"; 
  arg.description = "Query sequence";
  arg.required = false; 
  arg.defaultValueString = "ATCGAAGATCCATGT"; 
  arg.type = "string"; 
  arg.multi = false; 
  ArgList.push_back(arg);
  ValueArg<string> cmd_query(arg.shortId, arg.longId, arg.description, arg.required, arg.defaultValueString, arg.type, cmd);

  // match score
  ArgStruct argM;
  arg = argM;
  arg.shortId = "";
  arg.longId = "M";
  arg.description = "Match score";
  arg.required = false;
  arg.defaultValueString = "2";
  arg.type = "int";
  arg.multi = false;
  ArgList.push_back(arg);
  ValueArg<int> cmd_M(arg.shortId, arg.longId, arg.description, arg.required, 2, arg.type, cmd);

  // mismatch score
  ArgStruct argX;
  arg = argX;
  arg.shortId = "";
  arg.longId = "X";
  arg.description = "Mismatch score";
  arg.required = false;
  arg.defaultValueString = "-2";
  arg.type = "int";
  arg.multi = false;
  ArgList.push_back(arg);
  ValueArg<int> cmd_X(arg.shortId, arg.longId, arg.description, arg.required, -2, arg.type, cmd);

  // gap initiation score
  ArgStruct argGI;
  arg = argX;
  arg.shortId = "";
  arg.longId = "GI";
  arg.description = "Gap initiation score";
  arg.required = false;
  arg.defaultValueString = "-3";
  arg.type = "int";
  arg.multi = false;
  ArgList.push_back(arg);
  ValueArg<int> cmd_GI(arg.shortId, arg.longId, arg.description, arg.required, -3, arg.type, cmd);

  // gap extension score
  ArgStruct argGE;
  arg = argX;
  arg.shortId = "";
  arg.longId = "GE";
  arg.description = "Gap extension score";
  arg.required = false;
  arg.defaultValueString = "-1";
  arg.type = "int";
  arg.multi = false;
  ArgList.push_back(arg);
  ValueArg<int> cmd_GE(arg.shortId, arg.longId, arg.description, arg.required, -1, arg.type, cmd);

  // print matrix?
  ArgStruct argMatrix;
  arg = argMatrix;
  arg.shortId = "";
  arg.longId = "matrix";
  arg.description = "Print score matrix?";
  arg.required = false;
  arg.defaultValueString = "false";
  arg.type = "switch";
  arg.multi = false;
  ArgList.push_back(arg);
  SwitchArg cmd_matrix(arg.shortId, arg.longId, arg.description, cmd, false);

  // debug
  ArgStruct argDebug;
  arg = argDebug;
  arg.shortId = "";
  arg.longId = "debug";
  arg.description = "Print debugging messages?";
  arg.required = false;
  arg.defaultValueString = "false";
  arg.type = "switch";
  arg.multi = false;
  ArgList.push_back(arg);
  SwitchArg cmd_debug(arg.shortId, arg.longId, arg.description, cmd, false);

  //----------------------------------------------------------------------------
  // register (but not add to cmd) special arguments that are automatically
  // added to cmd
  //----------------------------------------------------------------------------

  // help
  ArgStruct argHelp;
  arg = argHelp;
  arg.shortId = "h";
  arg.longId = "help";
  arg.description = "Print usage statement?";
  arg.required = false;
  arg.defaultValueString = "false";
  arg.type = "switch";
  arg.multi = false;
  ArgList.push_back(arg);

  // version
  ArgStruct argVersion;
  arg = argVersion;
  arg.shortId = "";
  arg.longId = "version";
  arg.description = "Print program version?";
  arg.required = false;
  arg.defaultValueString = "false";
  arg.type = "switch";
  arg.multi = false;
  ArgList.push_back(arg);

  //----------------------------------------------------------------------------
  // add custom output handler
  //----------------------------------------------------------------------------
  MyOutput my;
  cmd.setOutput(&my);

  //----------------------------------------------------------------------------
  // parse command line and catch possible errors
  //----------------------------------------------------------------------------
  try {
    cmd.parse(argc,argv);
  } 
  catch ( ArgException& e ) { 
    cerr << "ERROR: " << e.error() << " " << e.argId() << endl; 
  }
  
  //----------------------------------------------------------------------------
  // assign command line parameters
  //----------------------------------------------------------------------------

  string subject = cmd_subject.getValue();
  string query = cmd_query.getValue();
  int M = cmd_M.getValue();
  int X = cmd_X.getValue();
  int GI = cmd_GI.getValue();
  int GE = cmd_GE.getValue();
  bool matrix = cmd_matrix.getValue();
  bool debug = cmd_debug.getValue();

  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------
  // check and fix command line options
  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------

  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------
  // report command line and parameters used
  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------
  map<bool, string, less<bool> > bool2String;
  bool2String[false] = "false";
  bool2String[true] = "true";

  if (debug) {
    cerr << "Command line:";
    for (int i=0; i<argc; i++) {
      cerr << " " << argv[i];
    }
    cerr << endl;
    cerr << endl;
    cerr << "Complete list of parameter values:" << endl;
    cerr << "  --subject = " << subject << endl;
    cerr << "  --query = " << query << endl;
    cerr << "  --M = " << M << endl;
    cerr << "  --X = " << X << endl;
    cerr << "  --GI = " << GI << endl;
    cerr << "  --GE = " << GE << endl;
    cerr << "  --matrix = " <<  bool2String[matrix] << endl;
    cerr << "  --debug = " <<  bool2String[debug] << endl;
  }

  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------
  // main code starts here
  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------
  
  vector<Node *> subjectNodes;

  // make a node
  vector<Node *> contributors1;
  Node * node1 = new Node(
			  "node1",
			  "ATCGAGT",
			  contributors1,
			  0
			  );
  subjectNodes.push_back(node1);
 
  // make a node
  vector<Node *> contributors2;
  contributors2.push_back(node1);
  Node * node2 = new Node(
			  "node2",
			  "AATCC",
			  contributors2,
			  1
			  );
  subjectNodes.push_back(node2);

  // make a node
  vector<Node *> contributors3;
  contributors3.push_back(node1);
  Node * node3 = new Node(
			  "node3",
			  "A",
			  contributors3,
			  2
			  );
  subjectNodes.push_back(node3);
  
  // make a node
  vector<Node *> contributors4;
  contributors4.push_back(node2);
  contributors4.push_back(node3);
  Node * node4 = new Node(
			  "node4",
			  "GGGATCCAT",
			  contributors4,
			  0
			  );
  subjectNodes.push_back(node4);

  // make a node
  vector<Node *> contributors5;
  contributors4.push_back(node4);
  Node * node5 = new Node(
			  "node5",
			  "",
			  contributors4,
			  0
			  );
  subjectNodes.push_back(node5);
  

  GraphAlignment * ga;
  for (int i=1; i<2; i++) {
    ga = new GraphAlignment(subjectNodes, query, M, X, GI, GE, debug);
  }
  
  
  //Brute force refit for insertions
  /* for (vector<Node *>::const_iterator iter = subjectNodes.begin(); iter != subjectNodes.end(); iter++){
     Node * node = * iter;

     
     string nodeId = node->getId();
     string s1 = node->getSequence();
     int l1 = s1.length();
     vector<Node *> contributorNodes = node->getContributorNodes();

     int prev = ga->getScore();
     int nextScore = -1;
     
     cout << "\nref node is";
    node->printNode(cout);
    cout << "\n";
    
    if (node->isRef()){
    string c = string(1, s1.at(0));
    node->setSequence(s1.substr(1, l1));
      cout << "updated ref node is \n";
      node->printNode(cout);
      cout << "\n";
      
      contributorNodes[0]->setSequence(contributorNodes[0]->getSequence().append(c));
      nextScore = ga->getScore();
      } */
  
  for (vector<Node *>::const_iterator iter = subjectNodes.begin(); iter != subjectNodes.end(); iter++){
    Node * node = * iter;

    string nodeId = node->getId();
    string s1 = node->getSequence();
    int l1 = s1.length();
    vector<Node *> contributorNodes = node->getContributorNodes();
    int prevScore = ga->getScore();
    int nextScore = prevScore;
    
    /* if(node->isRef()){
      while(nextScore >= prevScore){
	prevScore = ga->getScore();
	if (node->getSequence().length() > 0){
	  node->pullFirst(contributorNodes[0], node);
	  cout << "node length: " << node->getSequence().length() << endl;
	}
	else {
	  break;
	}
	ga = updateGA(subjectNodes, query, M, X, GI, GE, debug);
	nextScore = ga->getScore();
	cout << "previous score is " << prevScore <<  endl;
	cout << "corrected score is: " << nextScore << endl;
      }
      if(prevScore > nextScore){
	node->undoPull(contributorNodes[0], node);
	ga = updateGA(subjectNodes, query, M, X, GI, GE, debug);
        int undo = ga->getScore();
	cout << "undo score is: " << undo << endl; 
      }
    } */

    if(node->isRef()){
      while(nextScore >= prevScore){
	prevScore = ga->getScore();
	Node *contributorNode = contributorNodes[0];
        if (contributorNode->getSequence().length() > 0){
	contributorNode->pushLast(contributorNode, node);  
	} else {
	  break;
	}
      
      ga = updateGA(subjectNodes, query, M, X, GI, GE, debug);
      nextScore = ga->getScore();
      cout << "previous push score is " << prevScore <<  endl;
      cout << "corrected push score is: " << nextScore << endl;
      }
      if(prevScore > nextScore) {
        Node *contributorNode = contributorNodes[0];
	contributorNode->undoPush(contributorNode, node);
	ga = updateGA(subjectNodes, query, M, X, GI, GE, debug); 
	int undo = ga->getScore();
        cout << "undo push score is: " << undo << endl;
      }
    }

    if(node->isSame() && contributorNodes.size() > 0){
      while(nextScore >= prevScore){
	prevScore = ga->getScore();
	if (node->getSequence().length() > 0){
	  contributorNodes[0]->pullFirst(contributorNodes[0], node);
	  //cout << "\n\n" << node->getSequence() << "\n\n";a
	} else {break;}
	
	cout << "\n\n" << node->getSequence() << "\n"; 
	cout << "" << contributorNodes[0]->getSequence() << "\n\n";

	ga = updateGA(subjectNodes, query, M, X, GI, GE, debug);
	nextScore = ga->getScore();

	cout << "previous pull score is: " << prevScore << endl;
	cout << "corrected pull score is: " << nextScore << endl;
      }

      if(prevScore > nextScore) {
	node->undoPull(contributorNodes[0], node);

	cout << "\n\n" << node->getSequence() << "\n";
        cout << "" << contributorNodes[0]->getSequence() << "\n\n";

	ga = updateGA(subjectNodes, query, M, X, GI, GE, debug);
	int undo = ga->getScore();
	cout << "undo pull score is: " << undo << endl;
      }
    }
  }
  
  
  cout << "Optimal score of GSW: " << ga->getScore() << endl;
  cout << "Global Cigar:" << ga->getGlobalCigar() << endl;
  cout << "Global Alignment:" << endl << ga->getGlobalAlignment() << endl;
  
  vector<Node *> matchedNodes = ga->getMatchedNodes();
  cout << "Graph node alignments:" << endl;
  for (vector<Node *>::const_iterator iter = matchedNodes.begin(); iter != matchedNodes.end(); iter++) {
    Node * node = * iter;
    string cigar = ga->getNodeCigar(node);
    int offset = ga->getNodeOffset(node);
    cout << "  Node=" << node->getId() << " CIGAR=" << cigar << " offset=" << offset << endl;
  }
}


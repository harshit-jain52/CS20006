#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define _social_node struct SocialNode
#define _type_node union TypeNode
#define _indiv_node struct Individual
#define _biz_node struct Business
#define _org_node struct Organisation
#define _grp_node struct Group
#define _links_ struct Links
#define _content_ struct Content
#define NAME_SIZE_LIMIT 25
#define CONTENT_SIZE_LIMIT 100
#define BUFSIZE 20
#define UI user.individual
#define UB user.business
#define UO user.organisation
#define UG user.group
#define clearscreen() printf("\e[H\e[2J\e[3J")
#define clearchars() while (getchar() != '\n')

/*
DESIGN IMPLEMENTATION:---------------------------------------------------------------------------------

* SocialNode : Struct representing every node.
* TypeNode: Union for selecting the type of node (individual, business, organisation or group)
* Individual, Business, Organisation, Group: Structs specific to the node type
* Links: Struct having reference to an instance of SocialNode.
* Content: Struct having reference to strings representing posts.
* NodeType: Enumeration of node types.

* Every node (user) is represented by an instance of SocialNode.
* Depending on the type of the user, the type-specific structs are utilised via instance of TypeNode.
* For a node, the set of links to other nodes is implemented by a linked-list of Links struct.
  Also, references to all the nodes are stored in a master linked-list, nodeList
  - Assumption: A node can be linked only to already established nodes,
    and linking can be done only at the time of creation of a node.
* For a node, posted content is implemented by a linked-list of Content struct.

FUNCTION IMPLEMENTATION:-------------------------------------------------------------------------------

* Creating Node:
- To keep user ID unique, a global variable uniqueID is incremented every time a node is created.
- Details like name, birthday and location are taken as input.
- Also, IDs of linked nodes are taken as input, and two-way links are established.
- The reference to created node is added in the nodeList.
- Note: Addition of link in a list is performed at the head of the linked-list
- Function(s) defined for this purpose:
  - createUser()
  - createIndividual()
  - createBusiness()
  - createOrganisation()
  - createGroup()
  - createLink()
  - pushLink()

* Deleting Node:
- Content posted by the node is deleted.
- References of the concerned node in the data of linked nodes are deleted, i.e., the links are removed.
- Reference of the node is deleted from the nodeList.
- Note: Deletion of link from a list is performed by comparing ID, algorithm is same as that for usual deletion of element in linked-list
- Function(s) defined for this purpose:
  - deleteUser()
  - deleteContent()
  - deleteRefInList()

* Searching Node:
- Searching is performed using the usual linear search
- Function(s) defined for this purpose:
  - searchByID()
  - searchByName()
  - searchByType()
  - searchByBirthday()

* Print 1-Hop Linked Nodes:
- In each node, depending on its type, lists of links are already present
- Every list is traversed and details of linked nodes are printed
- Function(s) defined for this purpose:
  - printUser()
  - printList()
  - printOneHopLinkedUsers()

* Posting Content:
- For the specified user, a string of content is taken as input
- If this string matches any of the already existing posts of the user, it is discarded to avoid duplicate posts
- Else a reference to this string is added to the list of posts of the user
- Function(s) defined for this purpose:
  - uploadPost()
  - createPost()
  - uploadPost()
  - searchContentInNode()

* Searching Content:
- A search-string is taken as input, which is compared with every post of every user
- Function(s) defined for this purpose:
  - searchContent()

* Displaying content of linked individuals
- For the specified individual, the lists of organisations and groups is traversed.
- For each organisation or group, content of each individual user is displayed
- Care is taken to prevent displaying the content of input individual
- Function(s) defined for this purpose:
  - displayLinkedContent()
  - displayListContent()
  - displayContent()

* Master Text-Based Interface
- Lists out the choices and calls functions according to the choice entered
- Function(s) defined for this purpose:
  - isNetworkOnline()
  - printMenu()
  - displayAll()
  - deleteNetwork()
*/

// Declarations of structs, union and enum--------------------------------------------------------------------------

struct SocialNode;
union TypeNode;
struct Individual;
struct Business;
struct Organisation;
struct Group;
struct Links;
struct Content;
struct Date;
struct Coordinates;
enum NodeType;

// Prototypes of Functions------------------------------------------------------------------------------------------

// Utility Functions

/*Check if the two strings are equal.
Comparison is case-insensitive*/
bool strcmpi(char *, char *);

/*Convert the string to lowercase*/
void strlower(char *);

/*Check if the 2nd string is a substring of the 1st.
Matching is case-insensitive*/
bool substr(char *, char *);

/*Check if the two dates are same*/
bool datecmp(struct Date, struct Date);

/*Take an integer input from stdin.
Prevent infinite loops or errors, set the value 0 if input is invalid*/
void getInt(int *);

// Functions for Linking nodes

/*Create an instance of _links_ struct having reference to the node*/
_links_ *createLink(_social_node *);

/*Add a link, with reference to the node, in the list of links*/
_links_ *pushLink(_links_ *, _social_node *);

// User Creation Functions

/*Create a new user account.
Returns if the account creation was successful*/
bool createUser();

/*Create an account for Individual*/
void createIndividual(_social_node *);

/*Create an account for Business*/
void createBusiness(_social_node *);

/*Create an account for Organisation*/
void createOrganisation(_social_node *);

/*Create an account for Group*/
void createGroup(_social_node *);

// User Searching Functions

/*Given an ID, search for the user in the social network*/
_social_node *searchByID(int, _links_ *);

/*Given a name, search for users in the social network*/
void searchByName(char[]);

/*List all users of the specified type*/
void searchByType(enum NodeType);

/*List all individuals having the given birth date*/
void searchByBirthday(struct Date);

// User Display Functions

/*Display details of the user*/
void printUser(_social_node *);

/*Display details of all users in the list*/
void printList(_links_ *);

/*Display details of all users linked to the given user*/
void printOneHopLinkedUsers(_social_node *);

// Functions for adding content

/*Create a post from the given string input*/
_content_ *createPost(char *);

/*Add the post to the content list of posts*/
_content_ *pushPost(_content_ *, char *);

/*Interface for uploading a post by the given user*/
void uploadPost(_social_node *);

// Content Searching Functions

/*Check if the given string is already posted by the user*/
bool searchContentInNode(_social_node *, char *);

/*Search for content across the social network*/
void searchContent(char[]);

// Content Display Functions

/*Display content of the user*/
void displayContent(_social_node *);

/*Display content of all the users in the list
(except for the user of given ID)*/
void displayListContent(_links_ *, int);

/*Display content of users linked to the given user through groups or/and organisations*/
void displayLinkedContent(_social_node *);

/*Display details (including content) of all Users*/
void displayAll();

// User Deletion Functions

/*Delete the reference to the user in the given list*/
_links_ *deleteRefInList(_links_ *, int);

/*Delete content posted by the user*/
void deleteContent(_social_node *);

/*Delete the user from the social network*/
void deleteUser(_social_node *);

/*Delete the whole social network of users*/
void deleteNetwork();

// Driver Menu

/*Master Text-Based User Interface*/
void printMenu();

/*Driver Function. Return if the social network is online*/
bool isNetworkOnline();

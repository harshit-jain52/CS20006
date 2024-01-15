#include "social.h"

// Definitions of structs, union and enum

typedef struct Date
{
    int day, month, year;
} Date;

typedef struct Coordinates
{
    float x, y;
} Coordinates;

typedef enum NodeType
{
    ind = 1,
    biz = 2,
    org = 3,
    grp = 4
} NodeType;

_indiv_node
{
    Date birthday;
    _links_ *groups;
    _links_ *orgs;
    _links_ *bizOwned;
    _links_ *bizConsumed;
};

_biz_node
{
    Coordinates location;
    _links_ *owners;
    _links_ *customers;
    _links_ *groups;
};

_org_node
{
    Coordinates location;
    _links_ *members;
};

_grp_node
{
    _links_ *members;
};

_social_node
{
    NodeType type;
    int id;
    char name[NAME_SIZE_LIMIT];
    Date creationDate;
    _content_ *posts;

    _type_node
    {
        _indiv_node individual;
        _biz_node business;
        _org_node organisation;
        _grp_node group;
    }
    user;
};

_links_
{
    _social_node *nodeRef;
    _links_ *next;
};

_content_
{
    char *post;
    _content_ *next;
};

// Global Variables

int uniqueID = 1;
_links_ *nodeList = NULL; // Master List
Date currdate = {16, 1, 2024};

// Function Definitions

void strlower(char *s)
{
    for (int i = 0; s[i] != '\0'; i++)
        s[i] = tolower(s[i]);
}

bool strcmpi(char *s1, char *s2)
{
    char s1_copy[CONTENT_SIZE_LIMIT + 1], s2_copy[CONTENT_SIZE_LIMIT + 1];
    strcpy(s1_copy, s1);
    strcpy(s2_copy, s2);
    strlower(s1_copy);
    strlower(s2_copy);

    return (strcmp(s1_copy, s2_copy) == 0);
}

bool substr(char *s, char *t)
{
    char t_copy[CONTENT_SIZE_LIMIT + 1], s_copy[CONTENT_SIZE_LIMIT + 1];
    strcpy(s_copy, s);
    strcpy(t_copy, t);
    strlower(s_copy);
    strlower(t_copy);

    return (strstr(s_copy, t_copy) != NULL);
}

bool datecmp(Date d1, Date d2)
{
    return (d1.day == d2.day && d1.month == d2.month && d1.year == d2.year);
}

void getInt(int *i)
{
    char buf[BUFSIZE];
    char *p;
    int input;
    if (fgets(buf, sizeof(buf), stdin) != NULL)
    {
        input = strtol(buf, &p, 10);
        if (buf[0] != '\n' && (*p == '\n' || *p == '\0'))
        {
            *i = input;
        }
        else
            *i = 0;
    }
    else
        *i = 0;
}

_links_ *createLink(_social_node *node)
{
    _links_ *link = (_links_ *)malloc(sizeof(_links_));
    link->nodeRef = node;
    link->next = NULL;

    return link;
}

_links_ *pushLink(_links_ *list, _social_node *node)
{
    if (node == NULL)
        return list;

    _links_ *newLink = createLink(node);
    newLink->next = list;
    return newLink;
}

bool createUser()
{
    _social_node *newNode = (_social_node *)malloc(sizeof(_social_node));
    int type;
    char name[NAME_SIZE_LIMIT];

    printf("\n1. Individual\n2. Business\n3. Organization\n4. Group");
    printf("\nType: ");
    getInt(&type);
    if (type < 1 || type > 4)
    {
        printf("Invalid Type");
        return false;
    }
    printf("Name: ");
    scanf(" %[^\n]s", name);

    newNode->id = uniqueID++;
    newNode->type = type;
    strcpy(newNode->name, name);
    newNode->posts = NULL;
    newNode->creationDate = currdate;
    switch (type)
    {
    case ind:
        createIndividual(newNode);
        break;
    case biz:
        createBusiness(newNode);
        break;
    case org:
        createOrganisation(newNode);
        break;
    case grp:
        createGroup(newNode);
        break;
    }
    nodeList = pushLink(nodeList, newNode);
    return true;
}

void createIndividual(_social_node *newNode)
{
    newNode->UI.bizOwned = NULL;
    newNode->UI.bizConsumed = NULL;
    newNode->UI.groups = NULL;
    newNode->UI.orgs = NULL;

    Date bday;
    printf("Enter birth date (DD MM YYYY): ");
    scanf("%d %d %d", &bday.day, &bday.month, &bday.year);
    newNode->UI.birthday = bday;

    clearchars();
    int num;
    printf("Enter no. of businesses owned: ");
    getInt(&num);
    while (num-- > 0)
    {
        int businessID;
        printf("Enter Business' ID (-1 to cancel): ");
        getInt(&businessID);
        if (businessID == -1)
            break;
        _social_node *business = searchByID(businessID, nodeList);
        if (business == NULL)
        {
            printf("Invalid ID\n");
            num++;
            continue;
        }
        if (business->type != biz)
        {
            printf("Not a business\n");
            num++;
            continue;
        }
        if (searchByID(businessID, newNode->UI.bizOwned))
        {
            printf("\nAlready Added\n");
            num++;
            continue;
        }
        business->UB.owners = pushLink(business->UB.owners, newNode);
        newNode->UI.bizOwned = pushLink(newNode->UI.bizOwned, business);
    }

    printf("Enter no. of businesses consuming: ");
    getInt(&num);
    while (num-- > 0)
    {
        int businessID;
        printf("Enter Business' ID (-1 to cancel): ");
        getInt(&businessID);
        if (businessID == -1)
            break;
        _social_node *business = searchByID(businessID, nodeList);
        if (business == NULL)
        {
            printf("Invalid ID\n");
            num++;
            continue;
        }
        if (business->type != biz)
        {
            printf("Not a business\n");
            num++;
            continue;
        }
        if (searchByID(businessID, newNode->UI.bizConsumed))
        {
            printf("\nAlready Added\n");
            num++;
            continue;
        }
        business->UB.customers = pushLink(business->UB.customers, newNode);
        newNode->UI.bizConsumed = pushLink(newNode->UI.bizConsumed, business);
    }

    printf("Enter no. of Groups the individual is part of: ");
    getInt(&num);
    while (num-- > 0)
    {
        int groupID;
        printf("Enter Group's ID (-1 to cancel): ");
        getInt(&groupID);
        if (groupID == -1)
            break;
        _social_node *group = searchByID(groupID, nodeList);
        if (group == NULL)
        {
            printf("Invalid ID\n");
            num++;
            continue;
        }
        if (group->type != grp)
        {
            printf("Not a group\n");
            num++;
            continue;
        }
        if (searchByID(groupID, newNode->UI.groups))
        {
            printf("\nAlready Added\n");
            num++;
            continue;
        }
        group->UG.members = pushLink(group->UG.members, newNode);
        newNode->UI.groups = pushLink(newNode->UI.groups, group);
    }

    printf("Enter no. of Organisations the individual is part of: ");
    getInt(&num);
    while (num-- > 0)
    {
        int orgID;
        printf("Enter Organisation's ID (-1 to cancel): ");
        getInt(&orgID);
        if (orgID == -1)
            break;
        _social_node *organisation = searchByID(orgID, nodeList);
        if (organisation == NULL)
        {
            printf("Invalid ID\n");
            num++;
            continue;
        }
        if (organisation->type != org)
        {
            printf("Not an organisation\n");
            num++;
            continue;
        }
        if (searchByID(orgID, newNode->UI.orgs))
        {
            printf("\nAlready Added\n");
            num++;
            continue;
        }
        organisation->UO.members = pushLink(organisation->UO.members, newNode);
        newNode->UI.orgs = pushLink(newNode->UI.orgs, organisation);
    }
}

void createBusiness(_social_node *newNode)
{
    newNode->UB.customers = NULL;
    newNode->UB.groups = NULL;
    newNode->UB.owners = NULL;

    Coordinates loc;
    printf("Enter location of business:\nX: ");
    scanf("%f", &loc.x);
    printf("Y: ");
    scanf("%f", &loc.y);
    newNode->UB.location = loc;

    clearchars();
    int num;
    printf("Enter no. of owners: ");
    getInt(&num);
    while (num-- > 0)
    {
        int ownerID;
        printf("Enter Owner's ID (-1 to cancel): ");
        getInt(&ownerID);
        if (ownerID == -1)
            break;
        _social_node *owner = searchByID(ownerID, nodeList);
        if (owner == NULL)
        {
            printf("Invalid ID\n");
            num++;
            continue;
        }
        if (owner->type != ind)
        {
            printf("Not an individual\n");
            num++;
            continue;
        }
        if (searchByID(ownerID, newNode->UB.owners))
        {
            printf("\nAlready Added\n");
            num++;
            continue;
        }
        owner->UI.bizOwned = pushLink(owner->UI.bizOwned, newNode);
        newNode->UB.owners = pushLink(newNode->UB.owners, owner);
    }

    printf("Enter no. of customers: ");
    getInt(&num);
    while (num-- > 0)
    {
        int customerID;
        printf("Enter Customer's ID (-1 to cancel): ");
        getInt(&customerID);
        if (customerID == -1)
            break;
        _social_node *customer = searchByID(customerID, nodeList);
        if (customer == NULL)
        {
            printf("Invalid ID\n");
            num++;
            continue;
        }
        if (customer->type != ind)
        {
            printf("Not an individual\n");
            num++;
            continue;
        }
        if (searchByID(customerID, newNode->UB.customers))
        {
            printf("\nAlready Added\n");
            num++;
            continue;
        }
        customer->UI.bizConsumed = pushLink(customer->UI.bizConsumed, newNode);
        newNode->UB.customers = pushLink(newNode->UB.customers, customer);
    }

    printf("Enter no. of Groups the business is part of: ");
    getInt(&num);
    while (num-- > 0)
    {
        int groupID;
        printf("Enter Group's ID (-1 to cancel): ");
        getInt(&groupID);
        if (groupID == -1)
            break;
        _social_node *group = searchByID(groupID, nodeList);
        if (group == NULL)
        {
            printf("Invalid ID\n");
            num++;
            continue;
        }
        if (group->type != grp)
        {
            printf("Not a group\n");
            num++;
            continue;
        }
        if (searchByID(groupID, newNode->UB.groups))
        {
            printf("\nAlready Added\n");
            num++;
            continue;
        }
        group->UG.members = pushLink(group->UG.members, newNode);
        newNode->UB.groups = pushLink(newNode->UB.groups, group);
    }
}

void createOrganisation(_social_node *newNode)
{
    newNode->UO.members = NULL;

    Coordinates loc;
    printf("Enter location of organisation:\nX: ");
    scanf("%f", &loc.x);
    printf("Y: ");
    scanf("%f", &loc.y);
    newNode->UO.location = loc;

    clearchars();
    int num;
    printf("Enter no. of members: ");
    getInt(&num);
    while (num-- > 0)
    {
        int memberID;
        printf("Enter Member's ID (-1 to cancel): ");
        getInt(&memberID);
        if (memberID == -1)
            break;
        _social_node *member = searchByID(memberID, nodeList);
        if (member == NULL)
        {
            printf("Invalid ID\n");
            num++;
            continue;
        }
        if (member->type != ind)
        {
            printf("Not an individual\n");
            num++;
            continue;
        }
        if (searchByID(memberID, newNode->UO.members))
        {
            printf("\nAlready Added\n");
            num++;
            continue;
        }
        member->UI.orgs = pushLink(member->UI.orgs, newNode);
        newNode->UO.members = pushLink(newNode->UO.members, member);
    }
}

void createGroup(_social_node *newNode)
{
    newNode->UG.members = NULL;

    clearchars();
    int num;
    printf("Enter no. of members: ");
    getInt(&num);
    while (num-- > 0)
    {
        int memberID;
        printf("Enter Member's ID (-1 to cancel): ");
        getInt(&memberID);
        if (memberID == -1)
            break;
        _social_node *member = searchByID(memberID, nodeList);
        if (member == NULL)
        {
            printf("Invalid ID\n");
            num++;
            continue;
        }
        if (member->type != ind && member->type != biz)
        {
            printf("Not an individual or a business\n");
            num++;
            continue;
        }
        if (searchByID(memberID, newNode->UG.members))
        {
            printf("\nAlready Added\n");
            num++;
            continue;
        }
        if (member->type == ind)
            member->UI.groups = pushLink(member->UI.groups, newNode);
        else
            member->UB.groups = pushLink(member->UB.groups, newNode);

        newNode->UG.members = pushLink(newNode->UG.members, member);
    }
}

_social_node *searchByID(int nodeID, _links_ *list)
{
    if (nodeID < 1 || nodeID > uniqueID)
        return NULL;
    _links_ *cur = list;
    while (cur)
    {
        if (cur->nodeRef->id == nodeID)
            return cur->nodeRef;
        cur = cur->next;
    }

    return NULL;
}

void searchByName(char nodeName[])
{
    int count = 0;
    _links_ *cur = nodeList;
    while (cur)
    {
        if (substr(cur->nodeRef->name, nodeName))
        {
            printUser(cur->nodeRef);
            count++;
        }
        cur = cur->next;
    }
    printf("\n%d User(s) found\n", count);
}

void searchByType(NodeType type)
{
    int count = 0;
    _links_ *cur = nodeList;
    while (cur)
    {
        if (cur->nodeRef->type == type)
        {
            printUser(cur->nodeRef);
            count++;
        }
        cur = cur->next;
    }
    printf("\n%d User(s) found\n", count);
}

void searchByBirthday(Date bday)
{
    int count = 0;
    _links_ *cur = nodeList;
    while (cur)
    {
        if (cur->nodeRef->type == ind && datecmp(cur->nodeRef->UI.birthday, bday))
        {
            printUser(cur->nodeRef);
            count++;
        }
        cur = cur->next;
    }
    printf("\n%d User(s) found\n", count);
}

void printUser(_social_node *node)
{
    printf("\nID: %d\tName: %s", node->id, node->name);
    switch (node->type)
    {
    case ind:
        printf("\tType: Individual\tBirthday: %d/%d/%d", node->UI.birthday.day, node->UI.birthday.month, node->UI.birthday.year);
        break;
    case biz:
        printf("\tType: Business\tLocation: (%f,%f)", node->UB.location.x, node->UB.location.y);
        break;
    case org:
        printf("\tType: Organisation\tLocation: (%f,%f)", node->UO.location.x, node->UO.location.y);
        break;
    case grp:
        printf("\tType: Group");
        break;
    }
}

void printList(_links_ *list)
{
    _links_ *cur = list;
    while (cur)
    {
        printUser(cur->nodeRef);
        cur = cur->next;
    }
}

void printOneHopLinkedUsers(_social_node *node)
{
    printf("\nUser:");
    printUser(node);

    switch (node->type)
    {
    case ind:
        printf("\nGroup(s):");
        printList(node->UI.groups);
        printf("\nOrganisation(s):");
        printList(node->UI.orgs);
        printf("\nBusiness(es) Owned:");
        printList(node->UI.bizOwned);
        printf("\nBusiness(es) Consuming From:");
        printList(node->UI.bizConsumed);
        break;
    case biz:
        printf("\nOwner(s):");
        printList(node->UB.owners);
        printf("\nCustomer(s):");
        printList(node->UB.customers);
        printf("\nGroup(s):");
        printList(node->UB.groups);
        break;
    case org:
        printf("\nMember(s):");
        printList(node->UO.members);
        break;
    case grp:
        printf("\nMember(s):");
        printList(node->UG.members);
        break;
    }
}

_content_ *createPost(char *newPost)
{
    _content_ *newContent = (_content_ *)malloc(sizeof(_content_));
    newContent->post = newPost;
    newContent->next = NULL;

    return newContent;
}

_content_ *pushPost(_content_ *list, char *post)
{
    _content_ *newContent = createPost(post);
    newContent->next = list;

    return newContent;
}

void uploadPost(_social_node *node)
{
    printUser(node);
    char tmp[CONTENT_SIZE_LIMIT + 1];
    printf("\nEnter content for the post (characters limit: %d):\n", CONTENT_SIZE_LIMIT);
    scanf(" %[^\n]s", tmp);
    clearchars();

    if (searchContentInNode(node, tmp))
    {
        printf("\nAlready Posted\n");
        return;
    }

    int contentSize = strlen(tmp) + 1;

    char *newPost = (char *)malloc(contentSize * sizeof(char));
    strcpy(newPost, tmp);

    node->posts = pushPost(node->posts, newPost);
    printf("Posted Successfully");
}

bool searchContentInNode(_social_node *node, char *content)
{
    _content_ *cur = node->posts;
    while (cur)
    {
        if (strcmpi(cur->post, content))
            return true;

        cur = cur->next;
    }

    return false;
}

void searchContent(char toSearch[])
{
    _links_ *cur = nodeList;

    int matches = 0;
    while (cur)
    {
        _content_ *trav = cur->nodeRef->posts;
        while (trav)
        {
            bool found = substr(trav->post, toSearch);
            if (found)
            {
                matches++;
                printUser(cur->nodeRef);
                printf("\n%s", trav->post);
            }
            trav = trav->next;
        }
        cur = cur->next;
    }

    printf("\n%d Match(es) found\n", matches);
}

void displayContent(_social_node *node)
{
    printUser(node);
    _content_ *cur = node->posts;
    if (cur == NULL)
        printf("\nNo content posted");
    while (cur)
    {
        printf("\n%s", cur->post);
        cur = cur->next;
    }
}

void displayListContent(_links_ *list, int exceptID)
{
    _links_ *cur = list;
    while (cur)
    {
        if (cur->nodeRef->id != exceptID && cur->nodeRef->type == ind)
            displayContent(cur->nodeRef);
        cur = cur->next;
    }
}

void displayLinkedContent(_social_node *node)
{
    if (node->type != ind)
    {
        printf("\nUser is not an individual");
        return;
    }

    printUser(node);
    _links_ *cur = NULL;

    cur = node->UI.groups;
    while (cur)
    {
        printf("\nGroup:");
        printUser(cur->nodeRef);
        displayListContent(cur->nodeRef->UG.members, node->id);
        printf("\n");
        cur = cur->next;
    }

    cur = node->UI.orgs;
    while (cur)
    {
        printf("\nOganization:");
        printUser(cur->nodeRef);
        displayListContent(cur->nodeRef->UO.members, node->id);
        cur = cur->next;
    }
}

void displayAll()
{
    _links_ *cur = nodeList;
    int ct = 0;
    while (cur)
    {
        displayContent(cur->nodeRef);
        printf("\n");
        cur = cur->next;
        ct++;
    }

    printf("\n%d User(s) Found", ct);
}

_links_ *deleteRefInList(_links_ *head, int nodeID)
{
    if (head == NULL)
        return head;

    if (head->nodeRef->id == nodeID)
    {
        _links_ *tmp = head;
        head = head->next;
        free(tmp);
        return head;
    }

    _links_ *tmp = head;
    _links_ *prev = NULL;

    while (tmp)
    {
        if (tmp->nodeRef->id == nodeID)
        {
            prev->next = tmp->next;
            free(tmp);
            break;
        }
        prev = tmp;
        tmp = tmp->next;
    }

    return head;
}

void deleteContent(_social_node *node)
{
    _content_ *cur = node->posts;
    _content_ *prev = NULL;

    while (cur)
    {
        prev = cur;
        cur = cur->next;
        free(prev);
    }

    node->posts = NULL;
}

void deleteUser(_social_node *node)
{
    deleteContent(node);

    int id = node->id;

    _links_ *cur = NULL;
    _links_ *prev = NULL;
    switch (node->type)
    {
    case ind:
        cur = node->UI.bizConsumed;
        while (cur)
        {
            cur->nodeRef->UB.customers = deleteRefInList(cur->nodeRef->UB.customers, id);
            prev = cur;
            cur = cur->next;
            free(prev);
        }

        cur = node->UI.groups;
        while (cur)
        {
            cur->nodeRef->UG.members = deleteRefInList(cur->nodeRef->UG.members, id);
            prev = cur;
            cur = cur->next;
            free(prev);
        }

        cur = node->UI.orgs;
        while (cur)
        {
            cur->nodeRef->UO.members = deleteRefInList(cur->nodeRef->UO.members, id);
            prev = cur;
            cur = cur->next;
            free(prev);
        }
        break;

    case biz:
        cur = node->UB.owners;
        while (cur)
        {
            cur->nodeRef->UI.bizOwned = deleteRefInList(cur->nodeRef->UI.bizOwned, id);
            prev = cur;
            cur = cur->next;
            free(prev);
        }

        cur = node->UB.customers;
        while (cur)
        {
            cur->nodeRef->UI.bizConsumed = deleteRefInList(cur->nodeRef->UI.bizConsumed, id);
            prev = cur;
            cur = cur->next;
            free(prev);
        }

        cur = node->UB.groups;
        while (cur)
        {
            cur->nodeRef->UG.members = deleteRefInList(cur->nodeRef->UG.members, id);
            prev = cur;
            cur = cur->next;
            free(prev);
        }
        break;

    case org:
        cur = node->UO.members;
        while (cur)
        {
            cur->nodeRef->UI.orgs = deleteRefInList(cur->nodeRef->UI.orgs, id);
            prev = cur;
            cur = cur->next;
            free(prev);
        }
        break;

    case grp:
        cur = node->UG.members;

        while (cur)
        {
            if (cur->nodeRef->type == ind)
                cur->nodeRef->UI.groups = deleteRefInList(cur->nodeRef->UI.groups, id);
            else
                cur->nodeRef->UB.groups = deleteRefInList(cur->nodeRef->UB.groups, id);

            prev = cur;
            cur = cur->next;
            free(prev);
        }
    }

    nodeList = deleteRefInList(nodeList, id);
}

void deleteNetwork()
{
    _links_ *cur = nodeList;
    _links_ *prev = NULL;
    while (cur)
    {
        prev = cur;
        cur = cur->next;
        deleteUser(prev->nodeRef);
    }
}

void printMenu()
{
    printf("\n--------------------------------------------------------------------------");
    printf("\n1. Create A User");
    printf("\n2. Delete A User");
    printf("\n3. Search for User");
    printf("\n4. Upload a post");
    printf("\n5. Display content of a user");
    printf("\n6. Search for content");
    printf("\n7. Display linked nodes");
    printf("\n8. Display content of linked individuals");
    printf("\n9. Display all the Content");
    printf("\n-1. Quit");
    printf("\n--------------------------------------------------------------------------\n");
}

bool isNetworkOnline()
{
    printMenu();
    int ch;
    printf("Enter choice: ");
    getInt(&ch);
    clearscreen();

    if (ch == 1)
    {
        if (createUser())
            printf("\nUser Account Created Successfully");
        else
            printf("\nError encountered while creating account");
    }
    else if (ch == 2)
    {
        int id;
        printf("\nEnter user ID: ");
        getInt(&id);
        _social_node *foundUser = searchByID(id, nodeList);
        if (foundUser)
        {
            deleteUser(foundUser);
            printf("\nUser Account Deleted Successfully");
        }
        else
            printf("\nAccount of entered ID not found");
    }
    else if (ch == 3)
    {
        printf("\n1. Search by ID\n2. Search by Name\n3. Search by type\n4. Search by birthday");
        int search;
        printf("\n\nEnter choice: ");
        getInt(&search);

        if (search == 1)
        {
            int id;
            printf("\nEnter user ID: ");
            getInt(&id);
            _social_node *foundUser = searchByID(id, nodeList);
            if (foundUser)
                printUser(foundUser);
            else
                printf("\nAccount of entered ID not found");
        }
        else if (search == 2)
        {
            char name[NAME_SIZE_LIMIT];
            printf("\nEnter User Name: ");
            scanf(" %[^\n]s", name);
            clearchars();
            searchByName(name);
        }
        else if (search == 3)
        {
            int type;
            printf("\n1. Individual\n2. Business\n3. Organization\n4. Group");
            printf("\nEnter user type: ");
            getInt(&type);
            if (type < 1 || type > 4)
                printf("Invalid Type\n");
            else
                searchByType(type);
        }
        else if (search == 4)
        {
            Date bday;
            printf("Enter birth date (DD MM YYYY): ");
            scanf("%d %d %d", &bday.day, &bday.month, &bday.year);
            searchByBirthday(bday);
            clearchars();
        }
        else
            printf("Invalid Input\n");
    }
    else if (ch == 4)
    {
        int id;
        printf("\nEnter user ID: ");
        getInt(&id);
        _social_node *foundUser = searchByID(id, nodeList);
        if (foundUser)
            uploadPost(foundUser);
        else
            printf("\nAccount of entered ID not found");
    }
    else if (ch == 5)
    {
        int id;
        printf("\nEnter user ID: ");
        getInt(&id);
        _social_node *foundUser = searchByID(id, nodeList);
        if (foundUser)
            displayContent(foundUser);
        else
            printf("\nAccount of entered ID not found");
    }
    else if (ch == 6)
    {
        char toSearch[CONTENT_SIZE_LIMIT + 1];
        printf("\nSearch: ");
        scanf(" %[^\n]s", toSearch);
        clearchars();
        searchContent(toSearch);
    }
    else if (ch == 7)
    {
        int id;
        printf("\nEnter user ID: ");
        getInt(&id);
        _social_node *foundUser = searchByID(id, nodeList);
        if (foundUser)
            printOneHopLinkedUsers(foundUser);
        else
            printf("\nAccount of entered ID not found");
    }
    else if (ch == 8)
    {
        int id;
        printf("\nEnter user ID: ");
        getInt(&id);
        _social_node *foundUser = searchByID(id, nodeList);
        if (foundUser)
            displayLinkedContent(foundUser);
        else
            printf("\nAccount of entered ID not found");
    }
    else if (ch == 9)
    {
        displayAll();
    }
    else if (ch == -1)
    {
        deleteNetwork();
        return false;
    }
    else
        printf("Invalid Input\n");

    return true;
}

int main()
{
    while (isNetworkOnline())
    {
        printf("\n\nPress ENTER to continue...");
        clearchars();
        clearscreen();
    }

    printf("\nQuitting...\n\nThank You for using the network!\n");
    return 0;
}
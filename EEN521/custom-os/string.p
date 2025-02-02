<<<<<<< .mine
# Author: Shane Salta, Zac Brown
# Date: 12.14.08
# Filename: string.p

export function strcpy, strcut, strcat, strlen, strchr, strrchr;
export function strlen_word, strcmp;
import function memset;

##############################################################
# strcpy: copies a string to another string from start to end offsets
# * arg 1: string to be copied FROM
# * arg 2: string to be copied TO, should have number of characters
#          to copy + 1 space available
# * arg 3: starting char position for stringin
# * arg 4: ending char position for stringin
# * returns: 
#   * failure - 0
#   * success - 0
##############################################################
function strcpy(stringin, stringout, start_offset, end_offset ) {
    local i, j;
    i = start_offset;
    j = 0;
    
    while (i <= end_offset) do
    {
        char j of stringout = char i of stringin;
        i = (i + 1);
        j = (j + 1)
    };
    
    char j of stringout = 0;
    return 0
}

##############################################################
# strcmp: compare specified number of charactesr between two strings
# * arg 1: string 1
# * arg 2: string 2
# * arg 3: number of characters to compare
# * returns: 
#   * string 1 lexicographically less than string 2     : -1
#   * string 1 lexicographically equal to string 2      :  0
#   * string 1 lexicographically greater than string 2  :  1
##############################################################
function strcmp(stringA, stringB, num_chars) {
    local i; 
    i = 0;
        
    while (i < num_chars) do {
        if (char i of stringA < char i of stringB) then return neg 1;
        if (char i of stringA > char i of stringB) then return 1;
        i = (i + 1)
    };
    
    return 0
}

##############################################################
# strcmp: compare specified number of charactesr between two strings
# * arg 1: string 1
# * arg 2: string 2
# * arg 3: number of characters to compare
# * returns: 
#   * failure - 0
#   * success - 0
##############################################################
function strcut(stringin, stringout, start_offset, end_offset, stringin_size ) {
    local i, j;
    i = start_offset;
    j = 0;
    while ( i <= end_offset) do
    {
        char j of stringout = char i of stringin;
        i = (i + 1);
        j = (j + 1)
    };
    i = start_offset;
    j = end_offset;
    if ( i < j) then
    {
        while ( j < stringin_size ) do
        {
            char i of stringin = char j of stringin;
            char j of stringin = 0;
            i = (i + 1);
            j = (j + 1)
        }
    };
    return 0
}

##############################################################
# strcat: adds a string to another string from start to end offsets
# * arg 1: string to be copied FROM
# * arg 2: string to be added TO
# * arg 3: starting char position for stringin
# * arg 4: ending char position for stringin
# * arg 5: char position to start append on stringout
# * returns:
#   * failure - 0
#   * success - 0
##############################################################
function strcat(stringin, stringout, start_offset, end_offset, 
        stringout_start) {
    local i, j;
    i = start_offset;
    j = stringout_start;
    while ( i <= end_offset) do
    {
        char j of stringout = char i of stringin;
        i = (i + 1);
        j = (j + 1)
    };
    return 0
}

##############################################################
# strlen: get length of specified string
# * arg 1: string to get length of
# * returns:
#   * failure - -1
#   * success - length of string
##############################################################
function strlen(stringin) {
    local ind;
    ind = 0;
    while (char ind of stringin != 0) do {
        ind = (ind + 1)
    };
    
    return ind
}

##############################################################
# strlen_word: get length of specified string in words
# * arg 1: string to get length of
# * returns:
#   * failure - -1
#   * success - length of string
##############################################################
function strlen_word(stringin) {
    local ind;
    ind = 0;
    while (char ind of stringin != 0) do {
        ind = (ind + 1)
    };
    
    return ((ind / 4) + 1)
}

##############################################################
# strrchr: find first occurence of specified character starting at end
# * arg 1: string to find character in
# * arg 2: character to search for
# * returns:
#   * failure - -1
#   * success - first location of character
##############################################################
function strrchr(stringin, search_char) {
    local ind;
    ind = call strlen(stringin);
    ind = (ind - 1);
    while (char ind of stringin != char 0 of search_char) do {
        ind = (ind - 1);
        if (ind = neg 1) then return neg 1
    };
    
    return ind
}

##############################################################
# strchr: find first occurence of specified character starting at beginning
# * arg 1: string to find character in
# * arg 2: character to search for
# * returns:
#   * failure - -1
#   * success - first location of character
##############################################################
function strchr(stringin, search_char) {
    local ind, last;
    ind = 0; 
    last = call strlen(stringin);
    
    while (char ind of stringin != char 0 of search_char) do {
        ind = (ind + 1);
        if (ind = last) then return neg 1
    };

    return ind
}

end
=======
# Author: Shane Salta, Zac Brown
# Date: 12.14.08
# Filename: string.p

export function strcpy, strcut, strcat, strlen, strchr, strrchr;
export function strlen_word, strcmp, find_next_space;
import function memset;

##############################################################
# strcpy: copies a string to another string from start to end offsets
# * arg 1: string to be copied FROM
# * arg 2: string to be copied TO, should have number of characters
#          to copy + 1 space available
# * arg 3: starting char position for stringin
# * arg 4: ending char position for stringin
# * returns: 
#   * failure - 0
#   * success - 0
##############################################################
function strcpy(stringin, stringout, start_offset, end_offset ) {
    local i, j;
    i = start_offset;
    j = 0;
    
    while (i <= end_offset) do
    {
        char j of stringout = char i of stringin;
        i = (i + 1);
        j = (j + 1)
    };
    
    char j of stringout = 0;
    return 0
}

##############################################################
# strcmp: compare specified number of charactesr between two strings
# * arg 1: string 1
# * arg 2: string 2
# * arg 3: number of characters to compare
# * returns: 
#   * string 1 lexicographically less than string 2     : -1
#   * string 1 lexicographically equal to string 2      :  0
#   * string 1 lexicographically greater than string 2  :  1
##############################################################
function strcmp(stringA, stringB, num_chars) {
    local i; 
    i = 0;
        
    while (i < num_chars) do {
        if (char i of stringA < char i of stringB) then return neg 1;
        if (char i of stringA > char i of stringB) then return 1;
        i = (i + 1)
    };
    
    return 0
}

##############################################################
# strcmp: compare specified number of charactesr between two strings
# * arg 1: string 1
# * arg 2: string 2
# * arg 3: number of characters to compare
# * returns: 
#   * failure - 0
#   * success - 0
##############################################################
function strcut(stringin, stringout, start_offset, end_offset, stringin_size ) {
    local i, j;
    i = start_offset;
    j = 0;
    while ( i <= end_offset) do
    {
        char j of stringout = char i of stringin;
        i = (i + 1);
        j = (j + 1)
    };
    i = start_offset;
    j = end_offset;
    if ( i < j) then
    {
        while ( j < stringin_size ) do
        {
            char i of stringin = char j of stringin;
            char j of stringin = 0;
            i = (i + 1);
            j = (j + 1)
        }
    };
    return 0
}

##############################################################
# strcat: adds a string to another string from start to end offsets
# * arg 1: string to be copied FROM
# * arg 2: string to be added TO
# * arg 3: starting char position for stringin
# * arg 4: ending char position for stringin
# * arg 5: char position to start append on stringout
# * returns:
#   * failure - 0
#   * success - 0
##############################################################
function strcat(stringin, stringout, start_offset, end_offset, 
        stringout_start) {
    local i, j;
    i = start_offset;
    j = stringout_start;
    while ( i <= end_offset) do
    {
        char j of stringout = char i of stringin;
        i = (i + 1);
        j = (j + 1)
    };
    return 0
}

##############################################################
# strlen: get length of specified string
# * arg 1: string to get length of
# * returns:
#   * failure - -1
#   * success - length of string
##############################################################
function strlen(stringin) {
    local ind;
    ind = 0;
    while (char ind of stringin != 0) do {
        ind = (ind + 1)
    };
    
    return ind
}

##############################################################
# strlen_word: get length of specified string in words
# * arg 1: string to get length of
# * returns:
#   * failure - -1
#   * success - length of string
##############################################################
function strlen_word(stringin) {
    local ind;
    ind = 0;
    while (char ind of stringin != 0) do {
        ind = (ind + 1)
    };
    
    return ((ind / 4) + 1)
}

##############################################################
# strrchr: find first occurence of specified character starting at end
# * arg 1: string to find character in
# * arg 2: character to search for
# * returns:
#   * failure - -1
#   * success - first location of character
##############################################################
function strrchr(stringin, search_char) {
    local ind;
    ind = call strlen(stringin);
    ind = (ind - 1);
    while (char ind of stringin != char 0 of search_char) do {
        ind = (ind - 1);
        if (ind = neg 1) then return neg 1
    };
    
    return ind
}

##############################################################
# strchr: find first occurence of specified character starting at beginning
# * arg 1: string to find character in
# * arg 2: character to search for
# * returns:
#   * failure - -1
#   * success - first location of character
##############################################################
function strchr(stringin, search_char) {
    local ind, last;
    ind = 0; 
    last = call strlen(stringin);
    
    while (char ind of stringin != char 0 of search_char) do {
        ind = (ind + 1);
        if (ind = last) then return neg 1
    };

    return ind
}

##############################################################
# find_next_space: given a string and a starting point, start, it
#    will find the next space in the array or /n and return its index
#
# * returns:
#   * failure - -1
#   * success - index of space or /n
##############################################################
function find_next_space(str, sizeof_str, start) {
    local end_str, i;
    end_str = -1;
    i = start;
    
    while ( i < sizeof_str) do
    {
        
        if ( (char i of str) = 32) then
        {#32 is ascii for space
            end_str = i;
            break;
        }else if ( (char i of str) = 13) then
        {#13 is ascii for carriage return or /n 
            end_str = i;
            break;
        }else if ( (char i of str) = 10) then
        {#10 is ascii for line feed or /n
            end_str = i;
            break;
        }
        i = ( i + 1)
    };
    
    return end_str
}
end
>>>>>>> .r81

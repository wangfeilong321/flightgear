/* Copyright (c) 2007-2009 by Adalin B.V.
 * Copyright (c) 2007-2009 by Erik Hofman
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of (any of) the copyrightholder(s) nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL 
 * THE COPYRIGHT HOLDER BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __XML_CONFIG
#define __XML_CONFIG 1

#ifdef __cplusplus
extern "C" {
#endif

#undef XML_NONVALIDATING

#ifdef XML_USE_NODECACHE
#include "xml_cache.h"
#else
void *cacheGet(void *);
#endif

enum
{
    XML_NO_ERROR = 0,
    XML_OUT_OF_MEMORY,
    XML_FILE_NOT_FOUND,
    XML_INVALID_NODE_NAME,
    XML_UNEXPECTED_EOF,
    XML_TRUNCATE_RESULT,
    XML_INVALID_COMMENT,
    XML_INVALID_INFO_BLOCK,
    XML_ELEMENT_NO_OPENING_TAG,
    XML_ELEMENT_NO_CLOSING_TAG,
    XML_ATTRIB_NO_OPENING_QUOTE,
    XML_ATTRIB_NO_CLOSING_QUOTE,
    XML_MAX_ERROR
};

#ifdef WIN32
# define WIN32_LEAN_AND_MEAN
# include <windows.h>

typedef struct
{
    HANDLE m;
    void *p;
} SIMPLE_UNMMAP;
#endif

#ifndef XML_NONVALIDATING
struct _xml_error
{
    char *pos;
    int err_no;
};
#endif

/*
 * It is required for both the rood node and the normal xml nodes to both
 * have 'char *name' defined as the first entry. The code tests whether
 * name == 0 to detect the root node.
 */
struct _root_id
{
    char *name;
    char *start;
    size_t len;
    int fd;
#ifdef XML_USE_NODECACHE
    void *node;
#endif
#ifndef XML_NONVALIDATING
    struct _xml_error *info;
#endif
#ifdef WIN32
    SIMPLE_UNMMAP un;
#endif
};

struct _xml_id
{
    char *name;
    char *start;
    size_t len;
    size_t name_len;
#ifndef XML_NONVALIDATING
    struct _root_id *root;
#endif
#ifdef XML_USE_NODECACHE
    void *node;
#endif
};



/**
 * Open an XML file for processing.
 *
 * @param fname path to the file 
 * @return XML-id which is used for further processing
 */
void *xmlOpen(const char *);

/**
 * Process a section of XML code in a preallocated buffer.
 * The buffer may not be free'd until xmlClose has been called.
 *
 * @param buffer pointer to the buffer
 * @param size size of the buffer
 * @return XML-id which is used for further processing
 */
void *xmlInitBuffer(const char *, size_t);

/**
 * Close the XML file after which no further processing is possible.
 *
 * @param xid XML-id
 */
void xmlClose(void *);


/**
 * Locate a subsection of the xml tree for further processing.
 * This adds processing speed since the reuired nodes will only be searched
 * in the subsection.
 *
 * The memory allocated for the XML-subsection-id has to be freed by the
 * calling process.
 *
 * @param xid XML-id
 * @param node path to the node containing the subsection
 * @return XML-subsection-id for further processing
 */
void *xmlNodeGet(const void *, const char *);

/**
 * Copy a subsection of the xml tree for further processing.
 * This is useful when it's required to process a section of the XML code
 * after the file has been closed. The drawback is the added memory
 * requirements.
 *
 * The memory allocated for the XML-subsection-id has to be freed by the
 * calling process.
 *
 * @param xid XML-id
 * @param node path to the node containing the subsection
 * @return XML-subsection-id for further processing
 */
void *xmlNodeCopy(const void *, const char *);


/**
 * Return the name of this node.
 * The returned string has to be freed by the calling process.
 *
 * @param xid XML-id
 * @return a newly alocated string containing the node name
 */
char *xmlNodeGetName(const void *);

/**
 * Copy the name of this node in a pre-allocated buffer.
 *
 * @param xid XML-id
 * @param buffer the buffer to copy the string to
 * @param buflen length of the destination buffer
 * @return the length of the node name
 */
size_t xmlNodeCopyName(const void *, char *, size_t);


/**
 * Create a marker XML-id that starts out with the same settings as the
 * refference XML-id.
 *
 * Marker id's are required when xmlNodeGetNum() and xmlNodeGetPos() are used
 * to walk a number of nodes. The xmlNodeGetPos function adjusts the contents
 * of the provided XML-id to keep track of it's position within the xml section.
 * The returned XML-id is limited to the boundaries of the requested XML tag
 * and has to be freed by the calling process.
 *
 * @param xid reference XML-id
 * @return a copy of the reference XML-id
 */
void *xmlMarkId(const void *);

/**
 * Get the number of nodes with the same name from a specified xml path.
 *
 * @param xid XML-id
 * @param path path to the xml node
 * @return the number count of the nodename
 */
unsigned int xmlNodeGetNum(const void *, const char *);

/**
 * Get the nth occurrence of node in the parent node.
 * The return value should never be altered or freed by the caller.
 *
 * @param pid XML-id of the parent node of this node
 * @param xid XML-id
 * @param node name of the node to search for
 * @param num specify which occurence to return
 * @return XML-subsection-id for further processing or NULL if unsuccessful
 */
void *xmlNodeGetPos(const void *, void *, const char *, size_t);


/**
 * Get a string of characters from the current node.
 * The returned string has to be freed by the calling process.
 *
 * @param xid XML-id
 * @return a newly alocated string containing the contents of the node
 */
char *xmlGetString(const void *);

/**
 * Get a string of characters from the current node.
 * This function has the advantage of not allocating its own return buffer,
 * keeping the memory management to an absolute minimum but the disadvantage
 * is that it's unreliable in multithread environments.
 *
 * @param xid XML-id
 * @param buffer the buffer to copy the string to
 * @param buflen length of the destination buffer
 * @return the length of the string
 */
size_t xmlCopyString(const void *, char *, size_t);

/**
 * Compare the value of this node to a reference string.
 * Comparing is done in a case insensitive way.
 *
 * @param xid XML-id
 * @param str the string to compare to
 * @return an integer less than, equal to, ro greater than zero if the value
 * of the node is found, respectively, to be less than, to match, or be greater
 * than str
 */
int xmlCompareString(const void *, const char *);

/**
 * Get a string of characters from a specified xml path.
 * The returned string has to be freed by the calling process.
 *
 * @param xid XML-id
 * @param path path to the xml node
 * @return a newly alocated string containing the contents of the node
 */
char *xmlNodeGetString(const void *, const char *);

/**
 * Get a string of characters from a specified xml path.
 * This function has the advantage of not allocating its own return buffer,
 * keeping the memory management to an absolute minimum but the disadvantage
 * is that it's unreliable in multithread environments.
 *
 * @param xid XML-id
 * @param path path to the xml node
 * @param buffer the buffer to copy the string to
 * @param buflen length of the destination buffer
 * @return the length of the string
 */
size_t xmlNodeCopyString(const void *, const char *, char *, size_t);

/**
 * Compare the value of a node to a reference string.
 * Comparing is done in a case insensitive way.
 *
 * @param xid XML-id
 * @param path path to the xml node to compare to
 * @param str the string to compare to
 * @return an integer less than, equal to, ro greater than zero if the value
 * of the node is found, respectively, to be less than, to match, or be greater
 * than str
 */
int xmlNodeCompareString(const void *, const char *, const char *);

/**
 * Get a string of characters from a named attribute.
 * The returned string has to be freed by the calling process.
 *
 * @param xid XML-id
 * @param name name of the attribute to acquire
 * @return the contents of the node converted to an integer value
 */
char *xmlAttributeGetString(const void *, const char *);

/**
 * Get a string of characters from a named attribute.
 * This function has the advantage of not allocating its own return buffer,
 * keeping the memory management to an absolute minimum but the disadvantage
 * is that it's unreliable in multithread environments.
 *
 * @param xid XML-id
 * @param name name of the attribute to acquire.
 * @param buffer the buffer to copy the string to
 * @param buflen length of the destination buffer
 * @return the length of the string
 */
size_t xmlAttributeCopyString(const void *, const char *, char *, size_t);

/**
 * Compare the value of an attribute to a reference string.
 * Comparing is done in a case insensitive way.
 *
 * @param xid XML-id
 * @param name name of the attribute to acquire.
 * @param str the string to compare to
 * @return an integer less than, equal to, ro greater than zero if the value
 * of the node is found, respectively, to be less than, to match, or be greater
 * than str
 */
int xmlAttributeCompareString(const void *, const char *, const char *);


/**
 * Get the integer value from the current node/
 *
 * @param xid XML-id
 * @return the contents of the node converted to an integer value
 */
long int xmlGetInt(const void *);

/**
 * Get an integer value from a specified xml path.
 *
 * @param xid XML-id
 * @param path path to the xml node
 * @return the contents of the node converted to an integer value
 */
long int xmlNodeGetInt(const void *, const char *);

/**
 * Get the integer value from the named attribute.
 *
 * @param xid XML-id
 * @param name name of the attribute to acquire
 * @return the contents of the node converted to an integer value
 */
long int xmlAttributeGetInt(const void *, const char *);


/**
 * Get the double value from the curent node/
 *
 * @param xid XML-id
 * @return the contents of the node converted to a double value
 */
double xmlGetDouble(const void *);

/**
 * Get a double value from a specified xml path/
 *
 * @param xid XML-id
 * @param path path to the xml node
 * @return the contents of the node converted to a double value
 */
double xmlNodeGetDouble(const void *, const char *);

/**
 * Get the double value from the named attribute.
 *
 * @param xid XML-id
 * @param name name of the attribute to acquire
 * @return the contents of the node converted to an integer value
 */
double xmlAttributeGetDouble(const void *, const char *);


/**
 * Get the error number of the last error and clear it.
 *
 * @param xid XML-id
 * @param clear clear the error state if non zero
 * @return the numer of the last error, 0 means no error detected.
 */
int xmlErrorGetNo(const void *, int);

/**
 * Get the line number of the last detected syntax error in the xml file.
 *
 * @param xid XML-id
 * @param clear clear the error state if non zero
 * @return the line number of the detected syntax error.
 */
size_t xmlErrorGetLineNo(const void *, int);

/**
 * Get the column number of the last detected syntax error in the xml file.
 *
 * @param xid XML-id
 * @param clear clear the error state if non zero
 * @return the line number of the detected syntax error.
 */
size_t xmlErrorGetColumnNo(const void *, int);

/**
 * Get a string that explains the last error.
 *
 * @param xid XML-id
 * @param clear clear the error state if non zero
 * @return a string that explains the last error.
 */
const char *xmlErrorGetString(const void *, int);

#ifdef __cplusplus
}
#endif

#endif /* __XML_CONFIG */

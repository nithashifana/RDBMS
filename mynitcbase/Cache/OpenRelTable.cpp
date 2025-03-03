// #include "OpenRelTable.h"
// #include <cstring>
// #include <iostream>
// #include <stdio.h>
// #include <stdlib.h>
// using namespace std;

// AttrCacheEntry *createAttrCacheEntryList(int size)
// {
//     AttrCacheEntry *head = nullptr, *curr = nullptr;
//     head = curr = (AttrCacheEntry *)malloc(sizeof(AttrCacheEntry));
//     size--;
//     while (size--)
//     {
//         curr->next = (AttrCacheEntry *)malloc(sizeof(AttrCacheEntry));
//         curr = curr->next;
//     }
//     curr->next = nullptr;
//     return head;
// }

// OpenRelTableMetaInfo OpenRelTable::tableMetaInfo[MAX_OPEN];
// OpenRelTable::OpenRelTable()
// {
//     // initialize relCache and attrCache with nullptr
//     for (int i = 0; i < MAX_OPEN; ++i)
//     {
//         RelCacheTable::relCache[i] = nullptr;
//         AttrCacheTable::attrCache[i] = nullptr;
//         tableMetaInfo[i].free = true;
//     }

//     /************ Setting up Relation Cache entries ************/
//     // (we need to populate relation cache with entries for the relation
//     RecBuffer relCatBlock(RELCAT_BLOCK);
//     Attribute relCatRecord[RELCAT_NO_ATTRS];
//     RelCacheEntry *relCacheEntry = nullptr;

//     for (int relId = RELCAT_RELID; relId <= ATTRCAT_RELID; relId++)
//     {
//         relCatBlock.getRecord(relCatRecord, relId);

//         relCacheEntry = (RelCacheEntry *)malloc(sizeof(RelCacheEntry));
//         RelCacheTable::recordToRelCatEntry(relCatRecord,&(relCacheEntry->relCatEntry));
//         relCacheEntry->recId.block = RELCAT_BLOCK;
//         relCacheEntry->recId.slot = relId;
//         relCacheEntry->searchIndex = {-1, -1};

//         RelCacheTable::relCache[relId] = relCacheEntry;
//     }

//     /************ Setting up Attribute cache entries ************/
//     // (we need to populate attribute cache with entries for the relation

//     // setting up the variables
//     RecBuffer attrCatBlock(ATTRCAT_BLOCK);
//     Attribute attrCatRecord[ATTRCAT_NO_ATTRS];
//     AttrCacheEntry *attrCacheEntry = nullptr, *head = nullptr;

//     for (int relId = RELCAT_RELID, recordId = 0; relId <= ATTRCAT_RELID; relId++)
//     {
//         int numberOfAttributes =RelCacheTable::relCache[relId]->relCatEntry.numAttrs;
//         head = createAttrCacheEntryList(numberOfAttributes);
//         attrCacheEntry = head;

//         while (numberOfAttributes--)
//         {
//             attrCatBlock.getRecord(attrCatRecord, recordId);

//             AttrCacheTable::recordToAttrCatEntry(attrCatRecord,&(attrCacheEntry->attrCatEntry));
//             attrCacheEntry->recId.slot = recordId++;
//             attrCacheEntry->recId.block = ATTRCAT_BLOCK;

//             attrCacheEntry = attrCacheEntry->next;
//         }

//         AttrCacheTable::attrCache[relId] = head;
//     }

//     tableMetaInfo[RELCAT_RELID].free = false;
//     tableMetaInfo[ATTRCAT_RELID].free = false;
//     strcpy(tableMetaInfo[RELCAT_RELID].relName, "RELATIONCAT");
//     strcpy(tableMetaInfo[ATTRCAT_RELID].relName, "ATTRIBUTECAT");
// }

// OpenRelTable::~OpenRelTable()
// {
//     for (int i = 2; i < MAX_OPEN; ++i)
//     {
//         if (!tableMetaInfo[i].free)
//         {
//             OpenRelTable::closeRel(i); // we will implement this function later
//         }
//     }
//     // free all the memory that you allocated in the constructor
// }

// int OpenRelTable::getRelId(char relName[ATTR_SIZE])
// {
//     for (int i = 0; i < MAX_OPEN; i++)
//     {
//         if (strcmp(relName, tableMetaInfo[i].relName) == 0 and tableMetaInfo[i].free == false)
//         {
//             return i;
//         }
//     }
//     return E_RELNOTOPEN;
// }

// int OpenRelTable::getFreeOpenRelTableEntry()
// {

//     for (int i = 0; i < MAX_OPEN; i++)
//     {
//         if (tableMetaInfo[i].free)
//         {
//             return i;
//         }
//     }
//     return E_CACHEFULL;
// }

// int OpenRelTable::openRel(char relName[ATTR_SIZE])
// {
//     // (checked using OpenRelTable::getRelId())
//     int relId = getRelId(relName);
//     if (relId >= 0)
//     {
//         // return that relation id;
//         return relId;
//     }

//     // TODO: find a free slot in the Open Relation Table
//     // TODO: using OpenRelTable::getFreeOpenRelTableEntry().

//     // let relId be used to store the free slot.
//     relId = OpenRelTable::getFreeOpenRelTableEntry();

//     if (relId == E_CACHEFULL)
//         return E_CACHEFULL;

//     /****** Setting up Relation Cache entry for the relation ******/

//     // search for the entry with relation name, relName,
//     // in the Relation Catalog using BlockAccess::linearSearch().
//     //* Care should be taken to reset the searchIndex of the relation RELCAT_RELID
//     //* before calling linearSearch().

//     // relcatRecId stores the rec-id of the relation `relName` in the Relation Catalog.
//     Attribute attrVal;
//     strcpy(attrVal.sVal, relName);
//     RelCacheTable::resetSearchIndex(RELCAT_RELID);

//     RecId relcatRecId = BlockAccess::linearSearch(RELCAT_RELID, RELCAT_ATTR_RELNAME, attrVal, EQ);

//     if (relcatRecId.block == -1 and relcatRecId.slot == -1)
//     {
//         //! the relation is not found in the Relation Catalog
//         return E_RELNOTEXIST;
//     }

//     // TODO: read the record entry corresponding to relcatRecId and create a relCacheEntry
//     // TODO: on it using RecBuffer::getRecord() and RelCacheTable::recordToRelCatEntry().
//     RecBuffer relationBuffer(relcatRecId.block);
//     Attribute relationRecord[RELCAT_NO_ATTRS];
//     RelCacheEntry *relCacheBuffer = nullptr;

//     relationBuffer.getRecord(relationRecord, relcatRecId.slot);

//     //* NOTE: make sure to allocate memory for the RelCacheEntry using malloc()
//     relCacheBuffer = (RelCacheEntry *)malloc(sizeof(RelCacheEntry));
//     RelCacheTable::recordToRelCatEntry(relationRecord, &(relCacheBuffer->relCatEntry));

//     // update the recId field of this Relation Cache entry to relcatRecId.
//     relCacheBuffer->recId.block = relcatRecId.block;
//     relCacheBuffer->recId.slot = relcatRecId.slot;

//     // use the Relation Cache entry to set the relId-th entry of the RelCacheTable.
//     RelCacheTable::relCache[relId] = relCacheBuffer;

//     /****** Setting up Attribute Cache entry for the relation ******/

//     /*iterate over all the entries in the Attribute Catalog corresponding to each
//     attribute of the relation relName by multiple calls of BlockAccess::linearSearch()
//     care should be taken to reset the searchIndex of the relation, ATTRCAT_RELID,
//     corresponding to Attribute Catalog before the first call to linearSearch().*/
//     // {
//     // 	let attrcatRecId store a valid record id an entry of the relation, relName,
//     // 	in the Attribute Catalog.
//     // 	RecId attrcatRecId;

//     // 	read the record entry corresponding to attrcatRecId and create an
//     // 	Attribute Cache entry on it using RecBuffer::getRecord() and
//     // 	AttrCacheTable::recordToAttrCatEntry().
//     // 	update the recId field of this Attribute Cache entry to attrcatRecId.
//     // 	add the Attribute Cache entry to the linked list of listHead.
//     // NOTE: make sure to allocate memory for the AttrCacheEntry using malloc()
//     // }

//     Attribute attrCatRecord[ATTRCAT_NO_ATTRS];

//     // let listHead be used to hold the head of the linked list of attrCache entries.
//     AttrCacheEntry *attrCacheEntry = nullptr, *head = nullptr;

//     int numberOfAttributes = RelCacheTable::relCache[relId]->relCatEntry.numAttrs;
//     head = createAttrCacheEntryList(numberOfAttributes);
//     attrCacheEntry = head;

//     RelCacheTable::resetSearchIndex(ATTRCAT_RELID);
//     // while (numberOfAttributes--)
//     for (int attr = 0; attr < numberOfAttributes; attr++)
//     {
//         // AttrCacheTable::resetSearchIndex(relId, attr);
//         RecId attrcatRecId = BlockAccess::linearSearch(ATTRCAT_RELID, RELCAT_ATTR_RELNAME, attrVal, EQ);

//         RecBuffer attrCatBlock(attrcatRecId.block);
//         attrCatBlock.getRecord(attrCatRecord, attrcatRecId.slot);

//         AttrCacheTable::recordToAttrCatEntry(
//             attrCatRecord,
//             &(attrCacheEntry->attrCatEntry));

//         attrCacheEntry->recId.block = attrcatRecId.block;
//         attrCacheEntry->recId.slot = attrcatRecId.slot;

//         attrCacheEntry = attrCacheEntry->next;
//     }

//     // set the relIdth entry of the AttrCacheTable to listHead.
//     AttrCacheTable::attrCache[relId] = head;

//     /****** Setting up metadata in the Open Relation Table for the relation******/

//     // update the relIdth entry of the tableMetaInfo with free as false and
//     // relName as the input.
//     tableMetaInfo[relId].free = false;
//     strcpy(tableMetaInfo[relId].relName, relName);

//     return relId;
// }

// int OpenRelTable::closeRel(int relId)
// {
//     if (relId == 0 or relId == 1)
//     {
//         return E_NOTPERMITTED;
//     }

//     if (relId < 0 || relId >= MAX_OPEN)
//     {
//         return E_OUTOFBOUND;
//     }
//     if (tableMetaInfo[relId].free)
//         return E_RELNOTOPEN;
//     if (AttrCacheTable::attrCache[relId] == nullptr)
//     {
//         return E_RELNOTOPEN;
//     }

//     free(RelCacheTable::relCache[relId]);
//     AttrCacheEntry *head = AttrCacheTable::attrCache[relId];
//     AttrCacheEntry *next = head->next;
//     while (next)
//     {
//         free(head);
//         head = next;
//         next = next->next;
//     }
//     free(head);

//     // free the memory allocated in the relation and attribute caches which was
//     // allocated in the OpenRelTable::openRel() function
//     tableMetaInfo[relId].free = true;
//     AttrCacheTable::attrCache[relId] = nullptr;
//     RelCacheTable::relCache[relId] = nullptr;
//     // update `tableMetaInfo` to set `relId` as a free slot
//     // update `relCache` and `attrCache` to set the entry at `relId` to nullptr

//     return SUCCESS;
// }
#include "OpenRelTable.h"
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

AttrCacheEntry *createAttrCacheEntryList(int size)
{
    AttrCacheEntry *head = nullptr, *curr = nullptr;
    head = curr = (AttrCacheEntry *)malloc(sizeof(AttrCacheEntry));
    size--;
    while (size--)
    {
        curr->next = (AttrCacheEntry *)malloc(sizeof(AttrCacheEntry));
        curr = curr->next;
    }
    curr->next = nullptr;
    return head;
}

OpenRelTableMetaInfo OpenRelTable::tableMetaInfo[MAX_OPEN];

OpenRelTable::OpenRelTable()
{
    for (int i = 0; i < MAX_OPEN; ++i)
    {
        RelCacheTable::relCache[i] = nullptr;
        AttrCacheTable::attrCache[i] = nullptr;
        tableMetaInfo[i].free = true;
    }

    RecBuffer relCatBlock(RELCAT_BLOCK);
    Attribute relCatRecord[RELCAT_NO_ATTRS];
    RelCacheEntry *relCacheEntry = nullptr;

    for (int relId = RELCAT_RELID; relId <= ATTRCAT_RELID; relId++)
    {
        relCatBlock.getRecord(relCatRecord, relId);

        relCacheEntry = (RelCacheEntry *)malloc(sizeof(RelCacheEntry));
        RelCacheTable::recordToRelCatEntry(relCatRecord, &(relCacheEntry->relCatEntry));
        relCacheEntry->recId.block = RELCAT_BLOCK;
        relCacheEntry->recId.slot = relId;
        RelCacheTable::relCache[relId] = relCacheEntry;
    }

    RecBuffer attrCatBlock(ATTRCAT_BLOCK);
    Attribute attrCatRecord[ATTRCAT_NO_ATTRS];
    AttrCacheEntry *attrCacheEntry = nullptr, *head = nullptr;

    for (int relId = RELCAT_RELID, recordId = 0; relId <= ATTRCAT_RELID; relId++)
    {
        int numberOfAttributes = RelCacheTable::relCache[relId]->relCatEntry.numAttrs;
        head = createAttrCacheEntryList(numberOfAttributes);
        attrCacheEntry = head;

        while (numberOfAttributes--)
        {
            attrCatBlock.getRecord(attrCatRecord, recordId);
            AttrCacheTable::recordToAttrCatEntry(attrCatRecord, &(attrCacheEntry->attrCatEntry));
            attrCacheEntry->recId.slot = recordId++;
            attrCacheEntry->recId.block = ATTRCAT_BLOCK;
            attrCacheEntry = attrCacheEntry->next;
        }
        AttrCacheTable::attrCache[relId] = head;
    }

    tableMetaInfo[RELCAT_RELID].free = false;
    tableMetaInfo[ATTRCAT_RELID].free = false;
    strcpy(tableMetaInfo[RELCAT_RELID].relName, "RELATIONCAT");
    strcpy(tableMetaInfo[ATTRCAT_RELID].relName, "ATTRIBUTECAT");
}

OpenRelTable::~OpenRelTable()
{
    for (int i = 2; i < MAX_OPEN; ++i)
    {
        if (!tableMetaInfo[i].free)
        {
            OpenRelTable::closeRel(i);
        }
    }
}

int OpenRelTable::getRelId(char relName[ATTR_SIZE])
{
    for (int i = 0; i < MAX_OPEN; i++)
    {
        if (strcmp(relName, tableMetaInfo[i].relName) == 0 && !tableMetaInfo[i].free)
        {
            return i;
        }
    }
    return E_RELNOTOPEN;
}

int OpenRelTable::getFreeOpenRelTableEntry()
{
    for (int i = 0; i < MAX_OPEN; i++)
    {
        if (tableMetaInfo[i].free)
        {
            return i;
        }
    }
    return E_CACHEFULL;
}

int OpenRelTable::openRel(char relName[ATTR_SIZE])
{
    int relId = getRelId(relName);
    if (relId >= 0)
    {
        return relId;
    }
    relId = OpenRelTable::getFreeOpenRelTableEntry();
    if (relId == E_CACHEFULL)
        return E_CACHEFULL;

    Attribute attrVal;
    strcpy(attrVal.sVal, relName);
    RelCacheTable::resetSearchIndex(RELCAT_RELID);
    RecId relcatRecId = BlockAccess::linearSearch(RELCAT_RELID, RELCAT_ATTR_RELNAME, attrVal, EQ);

    if (relcatRecId.block == -1 && relcatRecId.slot == -1)
    {
        return E_RELNOTEXIST;
    }

    RecBuffer relationBuffer(relcatRecId.block);
    Attribute relationRecord[RELCAT_NO_ATTRS];
    RelCacheEntry *relCacheBuffer = (RelCacheEntry *)malloc(sizeof(RelCacheEntry));

    relationBuffer.getRecord(relationRecord, relcatRecId.slot);
    RelCacheTable::recordToRelCatEntry(relationRecord, &(relCacheBuffer->relCatEntry));
    relCacheBuffer->recId = relcatRecId;
    RelCacheTable::relCache[relId] = relCacheBuffer;

    AttrCacheEntry *attrCacheEntry = nullptr, *head = nullptr;
    int numberOfAttributes = relCacheBuffer->relCatEntry.numAttrs;
    head = createAttrCacheEntryList(numberOfAttributes);
    attrCacheEntry = head;

    RelCacheTable::resetSearchIndex(ATTRCAT_RELID);
    for (int attr = 0; attr < numberOfAttributes; attr++)
    {
        RecId attrcatRecId = BlockAccess::linearSearch(ATTRCAT_RELID, RELCAT_ATTR_RELNAME, attrVal, EQ);
        RecBuffer attrCatBlock(attrcatRecId.block);
        Attribute attrCatRecord[ATTRCAT_NO_ATTRS];
        attrCatBlock.getRecord(attrCatRecord, attrcatRecId.slot);

        AttrCacheTable::recordToAttrCatEntry(attrCatRecord, &(attrCacheEntry->attrCatEntry));
        attrCacheEntry->recId = attrcatRecId;
        attrCacheEntry = attrCacheEntry->next;
    }

    AttrCacheTable::attrCache[relId] = head;
    tableMetaInfo[relId].free = false;
    strcpy(tableMetaInfo[relId].relName, relName);

    return relId;
}

int OpenRelTable::closeRel(int relId)
{
    if (relId < 2 || relId >= MAX_OPEN || tableMetaInfo[relId].free)
        return E_RELNOTOPEN;

    free(RelCacheTable::relCache[relId]);
    AttrCacheEntry *head = AttrCacheTable::attrCache[relId];
    while (head)
    {
        AttrCacheEntry *temp = head->next;
        free(head);
        head = temp;
    }
    AttrCacheTable::attrCache[relId] = nullptr;
    RelCacheTable::relCache[relId] = nullptr;
    tableMetaInfo[relId].free = true;
    return SUCCESS;
}

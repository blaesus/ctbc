#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "parameters.h"
#include "datatypes.h"

// @see https://en.bitcoin.it/wiki/Protocol_documentation#version

struct VersionPayload {
    int32_t version;
    uint64_t services;
    int64_t timestamp;
    struct NetworkAddress addr_recv;

    // After version 106
    struct NetworkAddress addr_from;
    uint64_t nonce;
    struct VariableLengthString user_agent;
    int32_t start_height;

    // After version 70001
    bool relay;
};

union Payload {
    struct VersionPayload version;
};

typedef union Payload Payload;

// @see https://en.bitcoin.it/wiki/Protocol_documentation#Message_structure

#define CHECKSUM_SIZE 4
typedef uint8_t PayloadChecksum[CHECKSUM_SIZE];

struct Message {
    uint32_t magic;
    uint8_t command[12];
    uint32_t length; // of payload
    PayloadChecksum checksum;
    Payload *payload;
};

uint64_t serialize_version_message(
        struct Message *ptrMessage,
        uint8_t *ptrBuffer,
        uint32_t bufferSize
);
void make_verack_message(struct Message *ptrMessage);
uint8_t serialize_to_varint(uint64_t data, uint8_t *ptrBuffer);
uint64_t serialize_varstr(
        struct VariableLengthString *ptrVarStr,
        uint8_t *ptrBuffer
);
uint64_t serializeVersionPayload(
        struct VersionPayload *ptrPayload,
        uint8_t *ptrBuffer,
        uint32_t bufferSize
);
void printObjectWithLength(uint8_t *ptrData, uint64_t length);
uint32_t make_version_payload_to_peer(
        struct Peer *ptrPeer,
        struct VersionPayload *ptrPayload
);
void make_version_message(
        struct Message *ptrMessage,
        struct VersionPayload *ptrPayload,
        uint32_t payloadLength
);

uint64_t parseMessageHeader(
        uint8_t *buffer,
        struct Message *ptrMessage
);

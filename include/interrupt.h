#pragma once

#include <cstdint>
#include <cstddef>

void setupIntrDesc(size_t id, void* callback, uint16_t dpl);

void setupTrapDesc(size_t id, void* callback, uint16_t dpl);

void set8259Mask(uint8_t port);

void clear8259Mask(uint8_t port);
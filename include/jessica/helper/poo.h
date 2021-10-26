#pragma once

#define RULE_OF_FIVE_COPY(CLASS)           \
  CLASS(const CLASS&) = default;           \
  CLASS(CLASS&&) = delete;                 \
  CLASS& operator=(const CLASS&) = delete; \
  CLASS& operator=(CLASS&&) = delete;      \
  ~CLASS() = default;

#define RULE_OF_FIVE_COPY_VIRTUAL(CLASS)   \
  CLASS(const CLASS&) = default;           \
  CLASS(CLASS&&) = delete;                 \
  CLASS& operator=(const CLASS&) = delete; \
  CLASS& operator=(CLASS&&) = delete;      \
  virtual ~CLASS() = default;

#pragma once

#include <cstdint>
#include <queue>
#include <string>
#include <string_view>

class Reader;
class Writer;

class ByteStream
{
public:
  explicit ByteStream( uint64_t capacity );

  // Helper functions (provided) to access the ByteStream's Reader and Writer interfaces
  Reader& reader();
  const Reader& reader() const;
  Writer& writer();
  const Writer& writer() const;

  void set_error() { error_ = true; };       // Signal that the stream suffered an error.
  bool has_error() const { return error_; }; // Has the stream had an error?

protected:
  // Please add any additional state to the ByteStream here, and not to the Writer and Reader interfaces.
  std::queue<std::string> stream_ {};
  uint64_t remove_prefix_ {};
  uint64_t total_popped_ {};
  uint64_t total_pushed_ {};
  uint64_t total_buffered_ {};
  bool closed_ {};

  uint64_t capacity_;
  bool error_ {};
};

class Writer : public ByteStream
{
public:
  // 将数据推送到流中，但只能推送当前容量允许的数量。
  void push( std::string data );
  // 表示流已到达结尾。不会再写入更多内容。
  void close();

  // 流是否已关闭？
  bool is_closed() const;
  // 当前可以向流中推送多少字节？
  uint64_t available_capacity() const;
  // 累计向流中推送了多少字节
  uint64_t bytes_pushed() const;
};

class Reader : public ByteStream
{
public:
  // 查看缓冲区中的下一个字节数组
  std::string_view peek() const;
  // 从缓冲区中移除 `len` 字节
  void pop( uint64_t len );
  // 流是否已完成（关闭且完全移除）？
  bool is_finished() const;
  // 当前缓冲区中的字节数（已推送但未移除）
  uint64_t bytes_buffered() const;
  // 累计从流中移除了多少字节
  uint64_t bytes_popped() const;
};

/*
 * read: A (provided) helper function thats peeks and pops up to `len` bytes
 * from a ByteStream Reader into a string;
 */
void read( Reader& reader, uint64_t len, std::string& out );

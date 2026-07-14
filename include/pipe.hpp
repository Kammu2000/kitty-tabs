#pragma once

#include <array>

class Pipe
{
  public:
    Pipe();
    Pipe(const Pipe&) = delete;
    Pipe& operator=(const Pipe&) = delete;
    Pipe(Pipe&&) noexcept;
    Pipe& operator=(Pipe&&) noexcept;
    ~Pipe();

    int read_fd() const;
    int write_fd() const;
    void close_read();
    void close_write();

  private:
    inline void close_fd()
    {
        close_read();
        close_write();
    }

    std::array<int, 2> fd{-1, -1};
};

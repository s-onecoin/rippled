//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>

    Permission to use, copy, modify, and/or distribute this software for any
    purpose  with  or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    THE  SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH  REGARD  TO  THIS  SOFTWARE  INCLUDING  ALL  IMPLIED  WARRANTIES  OF
    MERCHANTABILITY  AND  FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY  SPECIAL ,  DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER  RESULTING  FROM  LOSS  OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION  OF  CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

    Portions based on SOCI - The C++ Database Access Library: 

    SOCI: http://soci.sourceforge.net/

    This file incorporates work covered by the following copyright
    and permission notice:

    Copyright (C) 2004 Maciej Sobczak, Stephen Hutton, Mateusz Loskot,
    Pawel Aleksander Fedorynski, David Courtney, Rafal Bobrowski,
    Julian Taylor, Henning Basold, Ilia Barahovski, Denis Arnaud,
    Daniel Lidstr�m, Matthieu Kermagoret, Artyom Beilis, Cory Bennett,
    Chris Weed, Michael Davidsaver, Jakub Stachowski, Alex Ott, Rainer Bauer,
    Martin Muenstermann, Philip Pemberton, Eli Green, Frederic Chateau,
    Artyom Tonkikh, Roger Orr, Robert Massaioli, Sergey Nikulov,
    Shridhar Daithankar, S�ren Meyer-Eppler, Mario Valesco.

    Boost Software License - Version 1.0, August 17th, 2003

    Permission is hereby granted, free of charge, to any person or organization
    obtaining a copy of the software and accompanying documentation covered by
    this license (the "Software") to use, reproduce, display, distribute,
    execute, and transmit the Software, and to prepare derivative works of the
    Software, and to permit third-parties to whom the Software is furnished to
    do so, all subject to the following:

    The copyright notices in the Software and this entire statement, including
    the above license grant, this restriction and the following disclaimer,
    must be included in all copies of the Software, in whole or in part, and
    all derivative works of the Software, unless such copies or derivative
    works are solely in the form of machine-executable object code generated by
    a source language processor.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
    SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
    FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
    ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/
//==============================================================================

#ifndef BEAST_SQDB_SESSION_H_INCLUDED
#define BEAST_SQDB_SESSION_H_INCLUDED

#include "../../../beast/smart_ptr/SharedPtr.h"
#include "../../beast_core/memory/SharedSingleton.h"

namespace beast {
namespace sqdb {

/*============================================================================*/
/**
  @brief A session for the embedded database.

  @ingroup vf_db
*/
class session
{
public:
    session();
    ~session();

    // DEPRECATED!
    // opens the deferred clone
    explicit session(session const& deferredClone);
    Error clone();

    /**
      Open a database.

      The database at the specified path is opened. The connection string is
      a set of "{key}={value}" pairs separated with the pipe symbol ('|').
      Choices for key and value are:

      @code

      timeout = (number) || "infinite"

      mode    = "read" || "write" || "create"

      cache   = "shared" || "private"

      threads = "single" || "multi"

      @endcode
    */
    Error open(String fileName,
               std::string options = "timeout=infinite|mode=create|threads=multi");

    void close();

    void begin();
    Error commit();
    void rollback();

    bool in_transaction()
    {
        return m_bInTransaction;
    }

    detail::once_type once(Error& error);
    detail::prepare_type prepare;

    rowid last_insert_rowid();

    std::ostringstream& get_query_stream();

    void log_query(std::string const& query);

    void set_got_data(bool bGotData);
    bool got_data() const;

    sqlite3* get_connection()
    {
        return m_connection;
    }

private:
    Error hard_exec(std::string const& query);

private:
    class Sqlite3;
    SharedPtr <SharedSingleton <Sqlite3> > m_instance;
    bool m_bInTransaction;
    sqlite3* m_connection;
    String m_fileName;
    std::string m_connectString;

    std::ostringstream m_query_stream;
    bool m_bGotData;
};

} // sqdb
} // beast

#endif

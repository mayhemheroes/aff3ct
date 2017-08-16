#ifndef SOCKET_HPP_
#define SOCKET_HPP_

#include <string>
#include <sstream>
#include <typeindex>

#include "Tools/Exception/exception.hpp"

namespace aff3ct
{
namespace module
{
class Process;

class Socket
{
	friend Process;

protected:
	Process &process;

	const std::string     name;
	const std::type_index datatype;
	const size_t          databytes;
	      void*           dataptr;

	Socket(Process &process, const std::string name, const std::type_index datatype, const size_t databytes,
	       void *dataptr = nullptr);

public:
	std::string     get_name           ();
	std::type_index get_datatype       ();
	std::string     get_datatype_string();
	uint8_t         get_datatype_size  ();
	size_t          get_databytes      ();
	size_t          get_n_elmts        ();
	void*           get_dataptr        ();
	void bind(Socket &s_in);
};
}
}

#endif /* SOCKET_HPP_ */

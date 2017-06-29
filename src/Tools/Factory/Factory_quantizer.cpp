#include <type_traits>

#include "Tools/Exception/exception.hpp"

#include "Module/Quantizer/Standard/Quantizer_standard.hpp"
#include "Module/Quantizer/Fast/Quantizer_fast.hpp"
#include "Module/Quantizer/Tricky/Quantizer_tricky.hpp"
#include "Module/Quantizer/NO/Quantizer_NO.hpp"

#include "Factory_quantizer.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename R, typename Q>
Quantizer<R,Q>* Factory_quantizer<R,Q>
::build(const std::string type,
        const int         size,
        const int         n_decimals,
        const int         n_bits,
        const float       sigma,
        const float       range,
        const int         n_frames)
{
	     if (type == "STD"     ) return new Quantizer_standard<R,Q>(size, n_decimals, n_bits,        n_frames);
	else if (type == "STD_FAST") return new Quantizer_fast    <R,Q>(size, n_decimals, n_bits,        n_frames);
	else if (type == "TRICKY"  ) return new Quantizer_tricky  <R,Q>(size, range,      n_bits, sigma, n_frames);
	else if (type == "NO"      ) return new Quantizer_NO      <R,Q>(size,                            n_frames);

	throw cannot_allocate(__FILE__, __LINE__, __func__);
}

template <typename R, typename Q>
void Factory_quantizer<R,Q>
::build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args)
{
	// ----------------------------------------------------------------------------------------------------- quantizer
	if (std::is_integral<Q>::value)
	{
		opt_args[{"qnt-type"}] =
			{"string",
			 "type of the quantizer to use in the simulation.",
			 "STD, STD_FAST, TRICKY"};

		opt_args[{"qnt-dec"}] =
			{"positive_int",
			 "the position of the fixed point in the quantified representation."};

		opt_args[{"qnt-bits"}] =
			{"positive_int",
			 "the number of bits used for the quantizer."};

		opt_args[{"qnt-range"}] =
			{"positive_float",
			 "the min/max bound for the tricky quantizer."};
	}
}

template <typename R, typename Q>
void Factory_quantizer<R,Q>
::store_args(const Arguments_reader& ar, quantizer_parameters &params)
{
	// ----------------------------------------------------------------------------------------------------- quantizer
	if (std::is_integral<Q>::value)
	{
		if(ar.exist_arg({"qnt-type" })) params.type       = ar.get_arg      ({"qnt-type" });
		if(ar.exist_arg({"qnt-dec"  })) params.n_decimals = ar.get_arg_int  ({"qnt-dec"  });
		if(ar.exist_arg({"qnt-bits" })) params.n_bits     = ar.get_arg_int  ({"qnt-bits" });
		if(ar.exist_arg({"qnt-range"})) params.range      = ar.get_arg_float({"qnt-range"});
	}
}

template <typename R, typename Q>
void Factory_quantizer<R,Q>
::group_args(Arguments_reader::arg_grp& ar)
{
	ar.push_back({"qnt", "Quantizer parameter(s)"});
}

template <typename R, typename Q>
void Factory_quantizer<R,Q>
::header(Header::params_list& head_qua, const quantizer_parameters& params)
{
	// ----------------------------------------------------------------------------------------------------- quantizer
	if (std::is_integral<Q>::value)
	{
		std::string quantif = "unused";
		if (! std::is_same<R,Q>::value)
		{
			if (params.type == "TRICKY")
				quantif = "{"+std::to_string(params.n_bits)+", "+std::to_string(params.range)+"f}";
			else
				quantif = "{"+std::to_string(params.n_bits)+", "+std::to_string(params.n_decimals)+"}";
		}

		head_qua.push_back(std::make_pair("Type"               , params.type));
		head_qua.push_back(std::make_pair("Fixed-point config.", quantif              ));
	}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template struct aff3ct::tools::Factory_quantizer<R_8,Q_8>;
template struct aff3ct::tools::Factory_quantizer<R_16,Q_16>;
template struct aff3ct::tools::Factory_quantizer<R_32,Q_32>;
template struct aff3ct::tools::Factory_quantizer<R_64,Q_64>;
#else
template struct aff3ct::tools::Factory_quantizer<R,Q>;
#endif
// ==================================================================================== explicit template instantiation

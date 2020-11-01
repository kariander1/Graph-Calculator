
#include "Exceptions.h"

namespace mtm
{

    const char *Exception::what() const noexcept
    {
        return error_string.c_str();
    }
        const char *FatalException::what() const noexcept
    {
        return error_string.c_str();
    }

} // namespace mtm

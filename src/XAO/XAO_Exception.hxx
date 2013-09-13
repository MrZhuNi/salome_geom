// Copyright (C) 2013  CEA/DEN, EDF R&D
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
// Author : Frederic Pons (OpenCascade)

#ifndef __XAO_EXCEPTION_HXX__
#define __XAO_EXCEPTION_HXX__

namespace XAO
{
    /**
     * \class XAO_Exception
     * Exception for XAO operations.
     */
    class XAO_Exception : public std::exception
    {
    public:
        /**
         * Default constructor.
         * @param message the exception message.
         */
        XAO_Exception(const char* message) : m_message(message)
        {
        }

        /**
         * Returns the error message.
         * @return the error message.
         */
        virtual const char* what() const throw ()
        {
            return m_message;
        }

    private:
        const char* m_message;
    };
}

#endif /* __XAO_EXCEPTION_HXX__ */

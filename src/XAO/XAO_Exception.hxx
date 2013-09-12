/*
 * XAO_Exception.hxx
 *
 *  Created on: 12 sept. 2013
 *      Author: salome
 */

#ifndef __XAO_EXCEPTION_HXX__
#define __XAO_EXCEPTION_HXX__

namespace XAO
{

    class XAO_Exception : public std::exception
    {
    public:
        XAO_Exception(const char* message) : m_message(message)
        {
        }

        virtual const char* what() const throw () { return m_message; }

    private:
        const char* m_message;
    };
}

#endif /* __XAO_EXCEPTION_HXX__ */

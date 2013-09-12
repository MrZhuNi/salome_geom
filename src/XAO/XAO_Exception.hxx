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

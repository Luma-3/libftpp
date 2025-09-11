#ifndef _MESSAGE_HPP
#define _MESSAGE_HPP

class Message
{
public:
    typedef int Type;

    Message(Type type);

private:
    Type _type;
};

#endif // !_MESSAGE_HPP

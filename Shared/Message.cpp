#include "Message.h"

Message::Message(std::string s, std::string d, bool isOwner, bool isWarning) : str(s), date(d), isOwner(isOwner), isWarning(isWarning) {}


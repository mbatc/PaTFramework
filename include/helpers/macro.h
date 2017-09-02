#pragma once

#define derive_crtp(base, derived) class derived : public base<derived>
#define TO_STR(x) #x
#pragma once

bool onButtonClicked (Rect Button)
{
    
    if (txMouseButtons () == 1)
    {
        if (txMouseX () > Button.left () && txMouseX () <  Button.right ())
        {
            if (txMouseY () > Button.top () && txMouseY () <  Button.bottom ())
            {
                while (txMouseButtons () == 1)
                {
                };
                if (txMouseX () > Button.left () && txMouseX () <  Button.right ())
                {
                    if (txMouseY () > Button.top () && txMouseY () <  Button.bottom ())
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;

    
}

bool inButtonMouse (Rect Button)
{

     if (txMouseX () > Button.left () && txMouseX () <  Button.right ())
     {
            if (txMouseY () > Button.top () && txMouseY () <  Button.bottom ())
            {
                return true;
            }
     }
     return false;
}
void KeyScan(void)
{
    static int keyCount = 0;
    static int keyState = 0;
    if (KEY_UP == 0 && keyState == 0) 
    {
        keyCount++;
        if (keyCount > 2 && KEY_UP == 0 && keyState == 0) 
        {
        	KeyUp();
            keyState = 1;
        }
    }
    else if (KEY_DOWN == 0 && keyState == 0)
    {
        keyCount++;
        if (keyCount > 2 && KEY_DOWN == 0 && keyState == 0)
        {
        	KeyDown();
            keyState = 1;
        }
    }
    else if (KEY_LEFT == 0 && keyState == 0)
    {
        keyCount++;
        if (keyCount > 2 && KEY_LEFT == 0 && keyState == 0)
        {
        	KeyLeft();
            keyState = 1;
        }
    }
    else if (KEY_RIGHT == 0 && keyState == 0)
    {
        keyCount++;
        if (keyCount > 2 && KEY_RIGHT == 0 && keyState == 0)
        {
        	KeyRight();
            keyState = 1;
        }
    }
    else if (KEY_ON == 0 && keyState == 0)
    {
        keyCount++;
        if (keyCount > 2 && KEY_ON == 0 && keyState == 0)
        {
        	KeyOk();
            keyState = 1;
        }
    }
    else if (KEY_OFF == 0 && keyState == 0)
    {
        keyCount++;
        if (keyCount > 2 && KEY_OFF == 0 && keyState == 0)
        {
            keyState = 1;
        }
    }
	else if (KEY_UP == 1 && KEY_DOWN == 1 && KEY_LEFT == 1 && KEY_RIGHT == 1 && keyState == 1) 
    {
        keyCount = 0;
        keyState = 0;
    }
}

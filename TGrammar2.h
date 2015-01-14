class TGrammar
{
	TGrammar ( void );
    ~TGrammar ( void );

    bool loadProduction ( const char* production );

    void parserPreprocess()
    {
        int a = 10;
        int b = a + 5;
    }

    void parseEnum();

    void fillUntil();

    int mID;
};



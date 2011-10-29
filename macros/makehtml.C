{

   THtml hh;
   hh.SetProductName("Swat");

   // Directory where the documentation will be built
   hh.SetOutputDir("htmldoc"); 

   hh.MakeIndex();
   hh.MakeAll();

}



void show_angles(Int_t t = 1)
{
   // Loop in the current directory and calls 
   // TEulerAngle::Show for all objects of the corresponding type.

   TIter iter(gDirectory->GetList());

   TObject* obj = NULL;
   while ((obj = iter.Next())){
      if (obj->IsA() == TEulerAngle::Class()){
         std::auto_ptr<TEulerAngle>(dynamic_cast<TEulerAngle*>(obj));
	 ang->Show(t);
      }
   }
}

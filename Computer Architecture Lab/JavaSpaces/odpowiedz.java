package javaspaceslab;

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author duszek
 */
import net.jini.core.entry.*;

 

public class odpowiedz implements Entry {
   public Integer index;
   public Integer wynik;

 

   public odpowiedz() {

   }

 

   public odpowiedz(int index, int wynik) {
     this.index = index;
     this.wynik = wynik;

   }

 

   public String toString() {

     return "Index: " + index +" Wynik: " +  wynik;

   }


   
//   public int GetIndex() {
//
//     return  index;
//
//   }

}



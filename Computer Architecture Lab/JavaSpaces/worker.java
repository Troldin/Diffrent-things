package javaspaceslab;

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author duszek
 */
import javaspaceslab.zadanie;
import javaspaceslab.odpowiedz;
import javaspaceslab.Lookup;
import net.jini.space.JavaSpace;

 

public class worker {
   public static int silnia (int n) {
        if (n==0)
            return 1;
        else
            return (n*silnia(n-1));
   }
   public static void main(String argv[]) {

      try {
         while(true){
         // Szukanie JavaSpaces
         Lookup finder = new Lookup(JavaSpace.class);

         JavaSpace space = (JavaSpace) finder.getService();
         //Pobranie pasujacego zadania  
         zadanie template = new zadanie();

         zadanie result = (zadanie) space.take(template, null, Long.MAX_VALUE);
         if(result.index == -1){
             space.write(result, null, 60*60*1000);
             break;
         }
         else{
         odpowiedz msg = new odpowiedz();
         //Stworzenie wpisu odpowiedzi z wynikiem
         msg = new odpowiedz();
         
         msg.wynik = silnia(result.number);
         msg.index = result.index;
         space.write(msg, null, 60*60*1000);
         System.out.println("Wykonaem zadanie!");
         }
         }
      } catch(Exception e) {

         e.printStackTrace();

      }

   }

}



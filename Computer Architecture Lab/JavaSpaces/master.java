package javaspaceslab;

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author duszek
 */
//import javaspaceslab.MessageEntry;
import javaspaceslab.zadanie;
import javaspaceslab.odpowiedz;
import javaspaceslab.Lookup;
import net.jini.space.JavaSpace;
import java.util.Random;




public class master {

   public static void main(String argv[]) {

      try {
        
        Random generator = new Random();
        int tab[] = new int[1000];
        for (int i=0; i<1000;i++)
        {
            tab[i] = generator.nextInt(10) + 1;
           //System.out.println(tab[i]);
        }
        Lookup finder = new Lookup(JavaSpace.class);
        JavaSpace space = (JavaSpace) finder.getService();
        for (int i=0; i<1000;i++)
        {
         zadanie msg = new zadanie();
         msg.number = tab[i];
         msg.index = i;
         //////////////////////////////////////////////////////

         //////////////////////////////////////////////////////
         System.out.println("The message write is: "+ msg);
         space.write(msg, null, 60*60*1000);
        }
        for (int i=0; i<1000;i++)
        {
         odpowiedz template = new odpowiedz();

         //System.out.println("Reading a message from the space...");
         
         odpowiedz result = (odpowiedz) space.take(template, null, Long.MAX_VALUE);
         tab[result.index] = result.wynik;
         System.out.println("The message read is: "+result);
        }
        ////////////////////  Trujaca pigulka   //////////////////////
        //System.out.println("TESSSSSSSSSST");
        zadanie toxic = new zadanie();
        toxic.number = null;
        toxic.index = -1;
        space.write(toxic, null, 10*1000);
      } catch(Exception e) {

         e.printStackTrace();

      }

   }

}



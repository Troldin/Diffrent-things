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

 

public class zadanie implements Entry {
   //public int[] number;
   public Integer index;
   public Integer number;
   
 

   public zadanie() {

   }


   public zadanie(int content, int index) {
      
     this.number = content; 
     this.index = index;
     //this.number[0] = content[0];
     //this.number[1] = content[1];
   
   }
   
public String toString() {

     return "Index: " + index +" Liczba: " +  number;

   }

}


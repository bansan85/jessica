import { NgModule } from '@angular/core';
import { ReactiveFormsModule } from '@angular/forms';
import { LibJessicaComponent } from './lib-jessica.component';

@NgModule({
  declarations: [LibJessicaComponent],
  imports: [ReactiveFormsModule],
  exports: [LibJessicaComponent]
})
export class LibJessicaModule {}

import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { MainComponent } from './ui/main/main.component';

import { LibJessicaModule } from 'lib-jessica';

@NgModule({
  declarations: [AppComponent, MainComponent],
  imports: [BrowserModule, AppRoutingModule, LibJessicaModule],
  providers: [],
  bootstrap: [AppComponent],
})
export class AppModule {}

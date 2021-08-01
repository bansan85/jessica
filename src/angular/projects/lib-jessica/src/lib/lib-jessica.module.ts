/* eslint-disable max-len */
import { CommonModule } from '@angular/common';
import { NgModule } from '@angular/core';
import { FormsModule, ReactiveFormsModule } from '@angular/forms';
import { MeyerhofFormComponent } from './data/geotechnical/meyerhof/meyerhof-form.component';
import { FoundationStripFormComponent } from './data/geotechnical/foundation-strip/foundation-strip-form.component';
import { VerticalEccentricFormComponent } from './data/load/vertical-eccentric/vertical-eccentric-form.component';
import { MeyerhofCalcComponent } from './calc/geotechnical/meyerhof-calc/meyerhof-calc.component';
/* eslint-enable max-len */

@NgModule({
  declarations: [
    MeyerhofFormComponent,
    FoundationStripFormComponent,
    VerticalEccentricFormComponent,
    MeyerhofCalcComponent
  ],
  imports: [CommonModule, FormsModule, ReactiveFormsModule],
  exports: [
    MeyerhofFormComponent,
    FoundationStripFormComponent,
    VerticalEccentricFormComponent,
    MeyerhofCalcComponent
  ]
})
export class LibJessicaModule {}

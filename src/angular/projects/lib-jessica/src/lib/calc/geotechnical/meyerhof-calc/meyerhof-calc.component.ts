/* eslint-disable max-len */
import { Component } from '@angular/core';
import { MeyerhofForm } from '../../../data/geotechnical/meyerhof/meyerhof-form';
import { MeyerhofCalcService } from './meyerhof-calc.service';
import { MeyerhofResult } from './meyerhof-result';
/* eslint-enable max-len */

@Component({
  selector: 'lib-meyerhof-calc',
  templateUrl: './meyerhof-calc.component.html',
  styleUrls: ['./meyerhof-calc.component.css']
})
export class MeyerhofCalcComponent {
  constructor(private calc: MeyerhofCalcService) {}

  compute(newItem: MeyerhofForm): void {
    const result: MeyerhofResult = this.calc.compute(newItem);
    console.log('qref : ' + result.qref + ', b\' : ' + result.b_);
  }
}

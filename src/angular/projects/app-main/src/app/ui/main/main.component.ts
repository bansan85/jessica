import { Component } from '@angular/core';

import { MeyerhofCalcService, MeyerhofCalc, MeyerhofResult } from 'jessica';
import { TranslateExService } from 'toolbox';

@Component({
  selector: 'app-main',
  templateUrl: './main.component.html',
  styleUrls: ['./main.component.css']
})
export class MainComponent {
  result!: MeyerhofResult;

  constructor(
    private calc: MeyerhofCalcService,
    public translateEx: TranslateExService
  ) {}

  compute(newItem: MeyerhofCalc): void {
    this.result = this.calc.compute(newItem.meyerhof);
  }
}

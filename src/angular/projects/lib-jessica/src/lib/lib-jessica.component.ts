import { Component, OnDestroy, OnInit } from '@angular/core';
import { FormControl, FormGroup, Validators } from '@angular/forms';
import * as Module from './jessica-web';
import { validNumber } from './util/validator/valid-number.validator';
import { Subscription } from 'rxjs';
import { debounceTime, filter, switchMap } from 'rxjs/operators';

@Component({
  selector: 'lib-jessica',
  templateUrl: './lib-jessica.component.html',
  styles: []
})
export class LibJessicaComponent implements OnInit, OnDestroy {
  private logger?: any;

  meyerhorForm: FormGroup;

  private obs$!: Subscription;

  submit(): void {
    console.log(Number(this.meyerhorForm.value.width));
    console.log(Number(this.meyerhorForm.value.load));
    console.log(Number(this.meyerhorForm.value.eccentric));
  }

  private instance?: any;

  constructor() {
    this.meyerhorForm = new FormGroup({
      width: new FormControl(null, [Validators.required, validNumber]),
      load: new FormControl(null, [Validators.required, validNumber]),
      eccentric: new FormControl(null, [Validators.required, validNumber])
    });
    Module.default().then(async (instance: any) => {
      this.instance = instance;
      this.logger = new this.instance.SpdlogStdoutMt('log');
    });
  }

  ngOnInit(): void {
    this.obs$ = this.meyerhorForm.valueChanges
      .pipe(debounceTime(200))
      .pipe(
        filter(() => this.meyerhorForm.valid && this.instance !== undefined),
        switchMap(() => {
          const foundation = new this.instance.FoundationStripRaw().setB(
            Number(this.meyerhorForm.value.width)
          );
          const load = new this.instance.VerticalEccentricRaw()
            .setV(Number(this.meyerhorForm.value.load))
            .setE(Number(this.meyerhorForm.value.eccentric));
          const calc = new this.instance.MeyerhofShallowFoundationDeco(
            this.logger,
            this.logger,
            load,
            foundation
          );

          return [{ a: calc.getQref(), b: calc.getB_() }];
        })
      )
      .subscribe((a) => console.log('123/' + a.a + ':' + a.b + '/456'));
  }

  ngOnDestroy(): void {
    this.obs$?.unsubscribe();
  }
}

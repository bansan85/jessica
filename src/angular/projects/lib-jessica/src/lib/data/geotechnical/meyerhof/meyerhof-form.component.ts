import {
  Component,
  EventEmitter,
  OnDestroy,
  OnInit,
  Output
} from '@angular/core';
import { FormBuilder, FormGroup } from '@angular/forms';
import { Subscription } from 'rxjs';
import { debounceTime, filter } from 'rxjs/operators';

import { MeyerhofForm } from './meyerhof-form';

@Component({
  selector: 'lib-meyerhof-form',
  templateUrl: './meyerhof-form.component.html',
  styleUrls: ['./meyerhof-form.component.css']
})
export class MeyerhofFormComponent implements OnInit, OnDestroy {
  form: FormGroup;
  private obs$!: Subscription;

  @Output() changeEvent = new EventEmitter<MeyerhofForm>();

  constructor(private fromBuilder: FormBuilder) {
    this.form = this.fromBuilder.group({
      foundation: [''],
      load: ['']
    });
  }

  ngOnInit(): void {
    this.obs$ = this.form.valueChanges
      .pipe(filter(() => this.form.valid))
      .pipe(debounceTime(200))
      .subscribe((a: MeyerhofForm) => {
        this.changeEvent.emit(a);
      });
  }

  ngOnDestroy(): void {
    this.obs$?.unsubscribe();
  }
}

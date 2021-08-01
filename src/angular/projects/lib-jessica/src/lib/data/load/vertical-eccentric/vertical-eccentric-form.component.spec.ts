/* eslint-disable max-len */
import { ComponentFixture, TestBed } from '@angular/core/testing';

import { VerticalEccentricFormComponent } from './vertical-eccentric-form.component';
/* eslint-enable max-len */

describe('VerticalEccentricFormComponent', () => {
  let component: VerticalEccentricFormComponent;
  let fixture: ComponentFixture<VerticalEccentricFormComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [VerticalEccentricFormComponent]
    }).compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(VerticalEccentricFormComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});